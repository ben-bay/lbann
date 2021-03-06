////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2016, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
// Written by the LBANN Research Team (B. Van Essen, et al.) listed in
// the CONTRIBUTORS file. <lbann-dev@llnl.gov>
//
// LLNL-CODE-697807.
// All rights reserved.
//
// This file is part of LBANN: Livermore Big Artificial Neural Network
// Toolkit. For details, see http://software.llnl.gov/LBANN or
// https://github.com/LLNL/LBANN.
//
// Licensed under the Apache License, Version 2.0 (the "Licensee"); you
// may not use this file except in compliance with the License.  You may
// obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the license.
////////////////////////////////////////////////////////////////////////////////

#include "lbann/proto/factories.hpp"
#include "lbann/trainers/trainer.hpp"
#include "lbann/callbacks/callback.hpp"

#include <trainer.pb.h>

namespace lbann {
namespace proto {

std::unique_ptr<trainer> construct_trainer(lbann_comm* comm,
                                           const std::map<execution_mode, generic_data_reader*>& data_readers,
                                           const lbann_data::Trainer& proto_trainer) {

  // Instantiate trainer
  auto t = make_unique<trainer>(comm, proto_trainer.mini_batch_size(), data_readers);
  const auto& name = proto_trainer.name();
  if (!name.empty()) {
    t->set_name(name);
  }

  for (auto d : data_readers) {
    d.second->set_trainer(t.get());
  }

  // Construct callbacks
  for (int i=0; i<proto_trainer.callback_size(); i++) {
    t->add_callback(construct_callback(proto_trainer.callback(i)));
  }

  return t;
}

} // namespace proto
} // namespace lbann
