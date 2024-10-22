from copy import deepcopy, copy
import logging
import numpy as np

logger = logging.getLogger(__name__)

class InstructionGenerator:
  def __init__(self):
    self.context = {}

  def print_work_instructions(self, work):
    for w in work.works:
      if isinstance(w, CMD_WRITE):
        data = w.data
        if isinstance(data, Instruction):
          print(f"PC : {w.addr}", end=" ")
          for key, value in work.labels.items():
            if value == w.addr:
              print(f"Label : {key}", end=" ")
          print(f"Inst : {OpCodeEnum(data.fields['opcode']).name}", end=" ")
          data.print_instruction()

  def allocate_register(self, name) -> int:
    if name in self.reg_map:
      logger.info("Register %s already exists. Using already allocated register %s", name, self.reg_map[name])

    for reg_id in range(1, self.reg_num + 1):
      if reg_id not in self.reg_map.values():
        self.reg_map[name] = reg_id
        return reg_id

    raise ValueError(f"No available register IDs. current allocated regmap: {self.reg_map}, reg_num: {self.reg_num}")

  def unallocate_register(self, name):
    if name not in self.reg_map:
      raise ValueError(f"Register {name} does not exist: cannot unallocate")
    self.reg_map.pop(name)

  def next_loop_register(self, name) -> int:
    loop_reg_id = deepcopy(self.loop_reg_id)
    self.loop_reg_id += 1
    self.reg_map[name] = loop_reg_id
    return loop_reg_id

def push_to_works(work, op, pc):
  work.append(CMD_WRITE(addr=pc, data=op))
  pc += 1
  return work, pc


class workSeq(list):
  def __init__(self, *args, **kwargs):
    super().__init__(*args, **kwargs)
  
  def add(self, *args):
    self.append(args)
  
  def get(self):
    return self

def dim_less_than_error_message(x, y):
  return f"For intrinsic conv node, the size of {x} dimension of weight tensor should be less than {y}"


def dim_in_error_message(x, y):
  return f"For intrinsic conv node, the size of {x} dimension of weight tensor should be one of {y}"


def is_pad_location(ih, iw, IH, IW, pad):
  is_left_pad = (iw < pad[1])
  is_right_pad = (iw >= (IW + pad[1]))
  is_bottom_pad = (ih >= (IH + pad[0]))
  is_top_pad = (ih < pad[0])

  is_pad = is_top_pad or is_left_pad or is_right_pad or is_bottom_pad
  return is_pad


def update_ifmap_spatial_coord(ih, iw, IH, IW, padding):
  max_x = IW + 2 * padding[0]
  max_y = IH + 2 * padding[1]

  iw += 1  # update x first,
  if (iw >= max_x):
    iw = 0  # reset x on overflow,
    ih += 1  # update y next

  if (ih >= max_y):
    iw = 0
    ih = 0

  return ih, iw

def get_in_read_cnt(IH, IW, paddings, strides, KH, KW):
  OH = int((IH - KH + 2 * paddings[0]) / strides[0]) + 1
  OW = int((IW - KW + 2 * paddings[1]) / strides[1]) + 1

  ofmap = np.zeros((OH, OW))
  current_ih, current_iw = 0, 0
  for oh in range(OH):
    for ow in range(OW):
      target_ih = oh * strides[0] + KH - 1
      target_iw = ow * strides[1] + KW - 1
      read_cnt = 0
      while True:
        last_iteration = (target_ih == current_ih) and (
          target_iw == current_iw)
        if is_pad_location(current_ih, current_iw, IH, IW, paddings):
          current_ih, current_iw = update_ifmap_spatial_coord(
            current_ih, current_iw, IH, IW, paddings)
        else:
          read_cnt += 1
          current_ih, current_iw = update_ifmap_spatial_coord(
            current_ih, current_iw, IH, IW, paddings)

        if last_iteration:
          break
      ofmap[oh, ow] = read_cnt

  return ofmap.astype(np.int32)


def get_conv_pattern(IH, IW, paddings, strides, KH, KW):
  ofmap_cnt_pattern = get_in_read_cnt(IH, IW, paddings, strides, KH, KW)
  OH, OW = ofmap_cnt_pattern.shape

  group = []
  prev_row = ofmap_cnt_pattern[0, :]
  current_pattern = {
    "pattern": prev_row,  # np array
    "start": 0,  # row start idx
    "end": 0  # row end idx
  }
  for oh in range(1, OH):
    current_row = ofmap_cnt_pattern[oh, :]
    if np.any(current_row != prev_row):
      group.append(current_pattern)
      current_pattern = {
        "pattern": current_row,  # np array
        "start": oh,  # row start idx
        "end": oh  # row end idx
      }
    else:
      current_pattern["end"] = oh

    prev_row = current_row

  group.append(current_pattern)
  # print(group)
  return group


def get_conv_in_row_pattern(conv_row_pattern):
  group = []
  prev_row = conv_row_pattern[0]
  current_pattern = {
    "pattern": prev_row,  # np array
    "start": 0,  # row start idx
    "end": 0  # row end idx
  }
  for ow in range(1, len(conv_row_pattern)):
    current_row = conv_row_pattern[ow]
    if current_row != prev_row:
      group.append(current_pattern)
      current_pattern = {
        "pattern": current_row,  # np array
        "start": ow,  # row start idx
        "end": ow  # row end idx
      }
    else:
      current_pattern["end"] = ow

    prev_row = current_row

  group.append(current_pattern)
  # print(group)
  return group


REGION_LOOP_THRESHOLD = 1
SLIDING_LOOP_THRESHOLD = 1
PIXEL_LOAD_LOOP_THRESHOLD = 1

class Conv2DCodeGenerator(InstructionGenerator):
  def __init__(self):
    super().__init__()
    self.var_cnt = 0 
    self.var_names = {}
    self.var_names["input_base_addr"] = self.getVarName()
    self.var_names["weight_base_addr"] = self.getVarName()
    self.var_names["output_base_addr"] = self.getVarName()
    self.var_names["offset"] = self.getVarName()
    self.var_names["branch_target"] = self.getVarName()
    self.var_names["psum_reg"] = self.getVarName()
    self.var_names["weight_0"] = self.getVarName()
    self.var_names["weight_1"] = self.getVarName()
    self.var_names["weight_2"] = self.getVarName()
    self.var_names["weight_3"] = self.getVarName()
    self.var_names["weight_4"] = self.getVarName()
    self.var_names["weight_5"] = self.getVarName()
    self.var_names["weight_6"] = self.getVarName()
    self.var_names["weight_7"] = self.getVarName()
    self.var_names["weight_8"] = self.getVarName()
    self.var_names["weight_9"] = self.getVarName()
    self.var_names["weight_10"] = self.getVarName()
    self.var_names["weight_11"] = self.getVarName()
    self.var_names["weight_12"] = self.getVarName()
    self.var_names["weight_13"] = self.getVarName()
    self.var_names["weight_14"] = self.getVarName()
    self.var_names["weight_15"] = self.getVarName()
    self.var_names["psum_dw"] = self.getVarName()
    self.var_names["sync_val"] = self.getVarName()

  def getVarName(self):
    cnt = self.var_cnt
    self.var_cnt += 1
    return f"var_{cnt}"

  def ofmap_pixel_inst_gen(self, work_seq: workSeq, load_num, is_accum, region_id, sub_row_id, dw_conv=False):
    if load_num > 0:
      if load_num >= PIXEL_LOAD_LOOP_THRESHOLD:
        # load and update
        work_seq.add("loop", load_num - 1, f"load_loop_start_{region_id}_{sub_row_id}")
        work_seq.add("LOAD_LB", self.in_recv_fid)
        work_seq.add("LOAD_LB", self.in_recv_fid)
        work_seq.add("LOAD_LB", self.in_recv_fid)
        work_seq.add("LOAD_LB", self.in_recv_fid)
        work_seq.add("loop_end", f"load_loop_start_{region_id}_{sub_row_id}")
      else:
        raise Exception("Not implemented")

    # step
    if dw_conv == False:
      compute_result = [self.getVarName() for _ in range(4)]
      work_seq.add("STEP", compute_result)

    # accum
    if is_accum:
      for i in range(4):
        psum_var = self.getVarName()
        work_seq.add("RECV", psum_var, self.psum_recv_fid)
        work_seq.add("vadd", psum_var, compute_result[i], psum_var)
        work_seq.add("SEND", psum_var, self.out_policy_addr, self.out_send_fid, 0)
      if self.with_sync:
        work_seq.add("addi", self.var_names["sync_val"], self.var_names["sync_val"], 1)
        work_seq.add("SET_FLAG", self.var_names["sync_val"])
    else:
      if dw_conv == False:
        if self.with_sync:
          work_seq.add("addi", self.var_names["sync_val"], self.var_names["sync_val"], 1)
          work_seq.add("SET_FLAG", self.var_names["sync_val"])
        for i in range(4):
          work_seq.add("SEND", compute_result[i], self.out_policy_addr, self.out_send_fid, 0)
      else:
        # * use first 16 input channel
        # * compute for 8 weight bit plane
        work_seq.add("DW_CONV", 0, 0, self.var_names["weight_0"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 1, 0, self.var_names["weight_1"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 2, 0, self.var_names["weight_2"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 3, 0, self.var_names["weight_3"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 4, 0, self.var_names["weight_4"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 5, 0, self.var_names["weight_5"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 6, 0, self.var_names["weight_6"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("DW_CONV", 7, 1, self.var_names["weight_7"], self.var_names["psum_dw"], 15, 0)
        work_seq.add("SEND", self.var_names["psum_dw"], self.out_policy_addr, self.out_send_fid, 0)

        # * next 16 input channels
        work_seq.add("DW_CONV", 0, 0, self.var_names["weight_8"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 1, 0, self.var_names["weight_9"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 2, 0, self.var_names["weight_10"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 3, 0, self.var_names["weight_11"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 4, 0, self.var_names["weight_12"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 5, 0, self.var_names["weight_13"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 6, 0, self.var_names["weight_14"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("DW_CONV", 7, 1, self.var_names["weight_15"], self.var_names["psum_dw"], 15, 1)
        work_seq.add("SEND", self.var_names["psum_dw"], self.out_policy_addr, self.out_send_fid, 0)

  def conv_row_inst_gen(self, work_seq: workSeq, pattern, is_accum, region_id, dw_conv=False):
    sub_row_patterns = get_conv_in_row_pattern(pattern)
    for i, sub_row_pattern in enumerate(sub_row_patterns): # multiple segment region
      loop_bound = sub_row_pattern["end"] - sub_row_pattern["start"] + 1
      load_num = sub_row_pattern["pattern"]
      if loop_bound >= SLIDING_LOOP_THRESHOLD:
        work_seq.add("loop", loop_bound-1, f"sliding_loop_start_{region_id}_{i}")
        self.ofmap_pixel_inst_gen(work_seq, load_num, is_accum, region_id, i, dw_conv)
        work_seq.add("loop_end", f"sliding_loop_start_{region_id}_{i}")
      else:
        raise Exception("Not implemented")

  def conv_row_region_inst_gen(self, work_seq: workSeq, pattern_group, is_accum, dw_conv=False):
    for i, region in enumerate(pattern_group): # multiple loop region
      loop_bound = region["end"] - region["start"] + 1
      if loop_bound >= REGION_LOOP_THRESHOLD:
        # loop region. iterate over multiple row
        work_seq.add("loop", loop_bound-1, f"region_loop_start_region_{i}")
        self.conv_row_inst_gen(work_seq, region["pattern"], is_accum, i, dw_conv)
        work_seq.add("loop_end", f"region_loop_start_region_{i}")
      else:
        raise Exception("Not implemented")

  def generate_compute(self, data_shape, weight_shape, stride, padding, in_recv_fid, out_send_fid, out_policy_addr, is_accum=False, dw_conv=False, psum_recv_fid=None, with_sync=False):
    """
      Parameters:
        - data_shape              : tuple -> input and output tensor data shape
        - weight_shape            : int   ->
        - in_recv_fid             : int   -> input tensor receive fid
        - param_recv_fid          : int   -> scale and bias receive fid. (scale, bias) pair
        - out_send_fid            : int   -> output tensor send fid
        - out_policy_addr         : int   -> output tensor policy address

      Required memory format:
        - in_data  : 16ic -> iw -> ih -> ic
        - out_data : 16oc -> ow -> oh -> oc
        - scale    : 16ic->ic
        - bias     : 16ic->ic
    """
    work_seq = workSeq()

    self.in_recv_fid = in_recv_fid
    self.out_send_fid = out_send_fid
    self.out_policy_addr = out_policy_addr
    self.psum_recv_fid = psum_recv_fid
    setattr(self, "with_sync", with_sync)

    IC, IH, IW = data_shape
    OC, _, KH, KW = weight_shape
    OH = ((IH - KH + 2 * padding) // stride) + 1
    OW = ((IW - KW + 2 * padding) // stride) + 1

    strides = [stride, stride]
    paddings = [padding, padding]

    if self.with_sync:
      self.var_names["sync_val"] = self.getVarName()
      work_seq.add("assign", self.var_names["sync_val"], 0)

    self.conv_row_region_inst_gen(work_seq, get_conv_pattern(IH, IW, paddings, strides, KH, KW), is_accum, dw_conv)

    return work_seq

  def generate_write_config(self, in_recv_fid):
    """
    """
    work_seq = workSeq()

    scan_reg0_id = self.allocate_register("scan_reg0")
    scan_reg1_id = self.allocate_register("scan_reg1")
    work_seq.add_ir(core_ir.RECV(fifo_id=in_recv_fid, rd=scan_reg0_id).set_tag("scan reg0 val"))
    work_seq.add_ir(core_ir.RECV(fifo_id=in_recv_fid, rd=scan_reg1_id).set_tag("scan reg1 val"))
    work_seq.add_ir(core_ir.SCAN_RW(rs1=scan_reg0_id, rd=scan_reg0_id).set_tag("scan first 256 bits"))
    work_seq.add_ir(core_ir.SCAN_RW(rs1=scan_reg1_id, rd=scan_reg1_id).set_tag("scan second 256 bits"))
    self.unallocate_register("scan_reg0")
    self.unallocate_register("scan_reg1")

    work_seq.add_ir(core_ir.RECV(fifo_id=in_recv_fid, rd=IMCE.reg_offset_map["cfg"]).set_tag(
      "config reg val"))  # config register write
    # work_seq.add_ir(core_ir.RECV(fifo_id=in_recv_fid, rd=0).set_tag("layer init"))  # layer init

    return work_seq

  def generate_write_weight(self, weight_shape, in_recv_fid):
    """
    """
    work_seq = workSeq()

    self.in_recv_fid = in_recv_fid
    self.compute_reg_id = IMCE.reg_offset_map["compute"]

    OC, _, KH, KW = weight_shape

    # * write weights to IMCU
    num_row = 256

    loop_start_label = LabelGenerator().get("loop_start")

    work_seq.add_ir(core_ir.RECV(fifo_id=in_recv_fid, rd=0).set_label(
      loop_start_label).set_tag("weight write start"))  # write IMCU
    work_seq.add_ir(core_ir.BRANCH_I_NEQ(
      self.reg_map["weight_loop"], num_row - 1, loop_start_label, update=True).set_tag("weight write end"))

    return work_seq

  def generate_write_dwconv_weight(self, weight_shape, in_recv_fid):
    """
    """
    work_seq = workSeq()

    self.in_recv_fid = in_recv_fid

    OC, _, KH, KW = weight_shape

    # * write dwconv weights to regfile
    # * 8b signed weight [32, 1, 3, 3]
    for i in range(16):
      work_seq.add_ir(core_ir.RECV(fifo_id=in_recv_fid, rd=self.reg_map[f"weight_{i}"]).set_tag(f"recv weight_{i}"))

    return work_seq
  
  def makeCode(self, work_seq):
    code = ""
    indent = 0
    get_indent = lambda x: " " * x

    code += "#include \"common_decl.h\"\n"

    code += "void conv2d() {\n"
    indent += 2

    loop_var_cnt = 0
    active_loops = []
    for work in work_seq:
      opcode = work[0]
      if opcode == "loop":
        loop_bound = work[1]
        loop_label = work[2]
        if loop_bound > 0:
          code += get_indent(indent) + \
            f"for (int i{loop_var_cnt} = 0; i{loop_var_cnt} < {loop_bound}; i{loop_var_cnt}++) {{ " + \
            f"// {loop_label}\n"
          indent += 2
          loop_var_cnt += 1
          active_loops.append(loop_label)
        else:
          code += get_indent(indent) + f"// {loop_label} unroll\n"
      elif opcode == "loop_end":
        if work[1] in active_loops:
          indent -= 2
          code += get_indent(indent) + "}\n"
      elif opcode == "assign":
        code += get_indent(indent) + f"short {work[1]} = {work[2]};\n"
      elif opcode == "LOAD_LB":
        code += get_indent(indent) + f"__builtin_IMCE_LOAD_LB({work[1]});\n"
      elif opcode == "STEP":
        code += get_indent(indent) + f"__builtin_IMCE_STEP();\n"
        code += get_indent(indent) + f"short16 {work[1][0]} = __builtin_IMCE_GET_CREG((short)0);\n"
        code += get_indent(indent) + f"short16 {work[1][1]} = __builtin_IMCE_GET_CREG((short)1);\n"
        code += get_indent(indent) + f"short16 {work[1][2]} = __builtin_IMCE_GET_CREG((short)2);\n"
        code += get_indent(indent) + f"short16 {work[1][3]} = __builtin_IMCE_GET_CREG((short)3);\n"
      elif opcode == "SET_FLAG":
        code += get_indent(indent) + f"__builtin_IMCE_SETFLAG({work[1]});\n"
      elif opcode == "SEND":
        code += get_indent(indent) + f"__builtin_IMCE_SEND({work[2]}, {work[1]}, {work[3]}, {work[4]});\n"
      elif opcode == "RECV":
        code += get_indent(indent) + f"short16 {work[1]} = __builtin_IMCE_RECV({work[2]});\n"
      elif opcode == "addi":
        typename = "short"
        if work[1] == work[2]:
          typename = ""
        if work[1] == work[3]:
          typename = ""
        code += get_indent(indent) + f"{typename} {work[1]} = {work[2]} + (short){work[3]};\n"
      elif opcode == "add":
        typename = "short"
        if work[1] == work[2]:
          typename = ""
        if work[1] == work[3]:
          typename = ""
        code += get_indent(indent) + f"{typename} {work[1]} = {work[2]} + {work[3]};\n"
      elif opcode == "vadd":
        typename = "short16"
        if work[1] == work[2]:
          typename = ""
        if work[1] == work[3]:
          typename = ""
        code += get_indent(indent) + f"{typename} {work[1]} = __builtin_IMCE_ADD({work[2]}, {work[3]}, 15);\n"
      elif opcode == "vaddi":
        typename = "short16"
        if work[1] == work[2]:
          typename = ""
        if work[1] == work[3]:
          typename = ""
        code += get_indent(indent) + f"{typename} {work[1]} = __builtin_IMCE_ADDI({work[2]}, (short){work[3]});\n"
    
    code += "}\n"
    return code

if __name__ == "__main__":
  gen = Conv2DCodeGenerator()
  data_shape = [16, 32, 32]
  weight_shape = [64, 16, 3, 3]
  stride = 1
  padding = 0
  in_recv_fid = 0
  out_send_fid = 2
  out_policy_addr = 3
  is_accum = True
  dw_conv = False
  psum_recv_fid = 5
  with_sync = True
  work_seq = gen.generate_compute(data_shape, weight_shape, stride, padding, in_recv_fid, out_send_fid, out_policy_addr, is_accum, dw_conv, psum_recv_fid, with_sync)

  code = gen.makeCode(work_seq)
  print(code)