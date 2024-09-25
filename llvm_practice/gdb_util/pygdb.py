import gdb

class BreakPointRunCommands(gdb.Breakpoint):
    def __init__(self, function_name, commands):
        # Set a breakpoint at the specified function
        super(BreakPointRunCommands, self).__init__(function_name, gdb.BP_BREAKPOINT, internal=False)
        self.function_name = function_name
        self.silent = True  # Make the breakpoint silent
        print("here")
        print(commands)
        self.commands = commands[0]
        print("here2")
        self.fd = open(f'{self.function_name}_log.txt', 'w')
    
    def stop(self):
        # Log the backtrace to a file when the breakpoint is hit
        for command in self.commands:
          self.fd.write(command + ':\n')
          result = gdb.execute(command, to_string=True)
          # backtrace = gdb.execute('bt', to_string=True)
          self.fd.write(result + '\n')
        
        # Continue program execution automatically
        return False
    

# Define a custom command class
class SetBreakpointAndRunCommands(gdb.Command):
    """Set a breakpoint at the function entry and log backtrace when hit."""

    def __init__(self):
        # Register the command with GDB under the name 'set_bp_and_log'
        super(SetBreakpointAndRunCommands, self).__init__("set_bp_and_log", gdb.COMMAND_USER)
    
    def invoke(self, arg, from_tty):
        # Parse the function name from the argument
        args = arg.split(" ")
        function_name = args[0]
        commands = args[1:]
        # print(function_name)
        # print(commands)

        if not function_name:
            print("Usage: set_bp_and_log <function_name>")
            return

        # Create a breakpoint using a custom breakpoint class
        BreakPointRunCommands(function_name, commands)
        print(f"Breakpoint set at {function_name}")

# Register the command in GDB
SetBreakpointAndRunCommands()

class WatchMemberVariable(gdb.Command):
    """Set a watchpoint on a specific member variable of a class instance."""

    def __init__(self):
        super(WatchMemberVariable, self).__init__("watch_member", gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        # Parse the command argument
        args = gdb.string_to_argv(arg)
        if len(args) != 1:
            print("Usage: watch_member variable")
            return

        varable_name = args[0]

        # Get the address of the member variable
        try:
            # Evaluate the member variable
            member_var = gdb.parse_and_eval(f'&({varable_name})')
            # Set a watchpoint on that memory address
            gdb.execute(f'watch *(int*){member_var}')
            print(f'Watchpoint set on {varable_name} at address {member_var}')
        except gdb.error as e:
            print(f"Error: {e}")

# Register the command with gdb
WatchMemberVariable()
