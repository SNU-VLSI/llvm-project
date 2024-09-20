import gdb

# Define a custom command class
class SetBreakpointAndLog(gdb.Command):
    """Set a breakpoint at the function entry and log backtrace when hit."""
    
    def __init__(self):
        # Register the command with GDB under the name 'set_bp_and_log'
        super(SetBreakpointAndLog, self).__init__("set_bp_and_log", gdb.COMMAND_USER)
    
    def invoke(self, arg, from_tty):
        # Parse the function name from the argument
        function_name = arg.strip()

        if not function_name:
            print("Usage: set_bp_and_log <function_name>")
            return

        # Create a breakpoint using a custom breakpoint class
        MyBreakpoint(function_name)
        print(f"Breakpoint set at {function_name}")

# Define a custom breakpoint class
class MyBreakpoint(gdb.Breakpoint):
    def __init__(self, function_name):
        # Set a breakpoint at the specified function
        super(MyBreakpoint, self).__init__(function_name, gdb.BP_BREAKPOINT, internal=False)
        self.silent = True  # Make the breakpoint silent
    
    def stop(self):
        # Log the backtrace to a file when the breakpoint is hit
        with open('backtrace.txt', 'a') as f:
            backtrace = gdb.execute('bt', to_string=True)
            f.write(backtrace + '\n')
        
        # Continue program execution automatically
        return False

# Register the command in GDB
SetBreakpointAndLog()

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
