//===--------------------- SourceMgr.h --------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
/// \file
/// This file implements class SourceMgr. Class SourceMgr abstracts the input
/// code sequence (a sequence of MCInst), and assings unique identifiers to
/// every instruction in the sequence.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_MCA_SOURCEMGR_H
#define LLVM_TOOLS_LLVM_MCA_SOURCEMGR_H

#include "llvm/MC/MCInst.h"
#include <vector>

namespace mca {

typedef std::pair<unsigned, const llvm::MCInst *> InstRef;

class SourceMgr {
  using InstVec = std::vector<std::unique_ptr<const llvm::MCInst>>;
  InstVec Sequence;
  unsigned Current;
  unsigned Iterations;
  static const unsigned DefaultIterations = 70;

public:
  SourceMgr(unsigned NumIterations)
      : Current(0),
        Iterations(NumIterations ? NumIterations : DefaultIterations) {}

  unsigned getCurrentIteration() const { return Current / Sequence.size(); }
  unsigned getNumIterations() const { return Iterations; }
  unsigned size() const { return Sequence.size(); }
  const InstVec &getSequence() const { return Sequence; }
  InstVec &getSequence() { return Sequence; }

  bool hasNext() { return Current < (Iterations * size()); }
  void updateNext() { Current++; }

  const InstRef peekNext() const {
    unsigned Index = getCurrentInstructionIndex();
    return InstRef(Current, Sequence[Index].get());
  }

  unsigned getCurrentInstructionIndex() const {
    return Current % Sequence.size();
  }

  const llvm::MCInst &getMCInstFromIndex(unsigned Index) const {
    return *Sequence[Index % size()];
  }

  bool isEmpty() const { return size() == 0; }
};
} // namespace mca

#endif
