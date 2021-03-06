#pragma once

#include <csignal>
#include <iostream>
#include <list>
#include <string>

#include <commands.hpp>
#include <cpu.hpp>
#include <hazard_detection.hpp>
#include <memory.hpp>

class CommandInterpreter {
 public:
  CommandInterpreter(CpuPtr cpu, MemoryPtr instr_mem, MemoryPtr data_mem,
                     std::istream& cmd_stream = std::cin);

  void MainLoop();

 private:
  class CommandFactory {
   public:
    CommandFactory(CpuPtr cpu, MemoryPtr instr_mem, MemoryPtr data_mem);

    enum Commands {
      Command_Help,
      Command_DumpRegisters,
      Command_DumpInstrMemory,
      Command_DumpDataMemory,
      Command_DumpCache,
      Command_Continue,
      Command_Step,
      Command_Reset,
      Command_Quit,
      Command_SetBreakpoint,
      Command_DeleteBreakpoint,
      Command_ShowBreakpoints,
      Command_Stats,
    };

    CommandPtr Create(std::string command_string);

   private:
    CpuPtr cpu_;
    MemoryPtr instr_mem_;
    MemoryPtr data_mem_;
  };

  std::istream& cmd_stream_;
  CpuPtr cpu_;
  MemoryPtr instr_mem_;
  MemoryPtr data_mem_;
  HazardDetectionPtr data_hazard_unit_;
  HazardDetectionPtr control_hazard_unit_;
  CommandFactory command_factory_;

  const std::string menu_string_{
      "\n\tdr [registers] : dump register contents\n"
      "\tdim [adress_range] : dump instruction memory contents\n"
      "\tddm [adress_range] : dump data memory contents\n"
      "\tdc [line range] : dump cache contents\n"
      "\tc : continue (runs program til completion)\n"
      "\ts [number cycles] : execute some number of cycles\n"
      "\tr : reset simulation\n"
      "\tq: quit\n"
      "\tbr [address] : set breakpoint\n"
      "\tdel [breakpoint number] : delete breakpoint\n"
      "\tsbr : show all breakpoints\n"
      "\tstat: show sim stats\n\n"};
};
