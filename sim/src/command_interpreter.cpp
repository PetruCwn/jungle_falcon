#include <command_interpreter.hpp>

#include <limits>

#include <commands.hpp>

////////////////////////////////////////////////////////////////////////////////
CommandInterpreter::CommandInterpreter(CpuPtr cpu, MemoryPtr instr_mem,
                                       MemoryPtr data_mem,
                                       std::istream& cmd_stream)
    : cmd_stream_(cmd_stream),
      cpu_(cpu),
      instr_mem_(instr_mem),
      data_mem_(data_mem),
      data_hazard_unit_(cpu->GetDataHazardDetector()),
      control_hazard_unit_(cpu->GetControlHazardDetector()),
      command_factory_(cpu, instr_mem, data_mem) {}

////////////////////////////////////////////////////////////////////////////////
void CommandInterpreter::MainLoop() {
  std::cout << "Beginning RISCV simulator (a.k.a. jungle_falcon)" << std::endl;
  do {
    std::cout << menu_string_;
    std::string input_str = "";
    std::cout << "\n>>> ";
    std::cin >> input_str;

    CommandPtr command = command_factory_.Create(input_str);
    if (command != nullptr) {
      command->RunCommand();
    } else {
      if (input_str == std::string("q")) {
        break;
      }
      VLOG(2) << "Unrecognized command: " << input_str;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<int>::max(), '\n');
    std::cout << std::endl;
  } while (true);
}

////////////////////////////////////////////////////////////////////////////////
CommandInterpreter::CommandFactory::CommandFactory(CpuPtr cpu,
                                                   MemoryPtr instr_mem,
                                                   MemoryPtr data_mem)
    : cpu_(cpu), instr_mem_(instr_mem), data_mem_(data_mem) {}

////////////////////////////////////////////////////////////////////////////////
CommandPtr CommandInterpreter::CommandFactory::Create(
    std::string command_string) {
  const std::size_t space_index =
      std::min(command_string.find(" ") - 1, command_string.length());
  const std::string command_op_string = command_string.substr(0, space_index);

  VLOG(5) << "Command Op String: " << command_op_string;

  std::map<std::string, Commands> COMMANDS{
      {std::string("h"), Command_Help},
      {std::string("dr"), Command_DumpRegisters},
      {std::string("dim"), Command_DumpInstrMemory},
      {std::string("ddm"), Command_DumpDataMemory},
      {std::string("c"), Command_Continue},
      {std::string("s"), Command_Step},
      {std::string("r"), Command_Reset},
      {std::string("q"), Command_Quit},
      {std::string("br"), Command_SetBreakpoint},
      {std::string("del"), Command_DeleteBreakpoint},
      {std::string("sbr"), Command_ShowBreakpoints},
      {std::string("stat"), Command_Stats}};

  // Check if command is valid
  const auto ite = COMMANDS.find(command_op_string);
  if (ite == COMMANDS.end()) {
    return nullptr;
  }

  const Commands command_id = COMMANDS[command_op_string];
  VLOG(5) << "Command ID: " << static_cast<int>(command_id);

  switch (command_id) {
    case Command_DumpRegisters:
      return std::make_shared<DumpRegistersCommand>(
          DumpRegistersCommand(command_string, cpu_->GetRegFile()));
    case Command_DumpInstrMemory:
      return std::make_shared<DumpMemoryCommand>(
          DumpMemoryCommand(command_string, instr_mem_));
    case Command_DumpDataMemory:
      return std::make_shared<DumpMemoryCommand>(
          DumpMemoryCommand(command_string, data_mem_));
    case Command_Continue:
      return std::make_shared<ContinueCommand>(
          ContinueCommand(command_string, cpu_));
    case Command_Step:
      return std::make_shared<StepCommand>(StepCommand(command_string, cpu_));
    case Command_Reset:
      return std::make_shared<ResetCommand>(ResetCommand(command_string, cpu_));
    case Command_Quit:
      return nullptr;
    case Command_SetBreakpoint:
      return std::make_shared<SetBreakpointCommand>(
          SetBreakpointCommand(command_string, cpu_));
    case Command_DeleteBreakpoint:
      return std::make_shared<DeleteBreakpointCommand>(
          DeleteBreakpointCommand(command_string, cpu_));
    case Command_ShowBreakpoints:
      return std::make_shared<ShowBreakpointsCommand>(
          ShowBreakpointsCommand(command_string, cpu_));
    case Command_Stats:
      return std::make_shared<ShowStatsCommand>(
          ShowStatsCommand(command_string, cpu_, cpu_->GetDataHazardDetector(),
                           cpu_->GetControlHazardDetector()));
    default:
      break;
  }
  return std::make_shared<NoActionCommand>(NoActionCommand(command_string));
}
