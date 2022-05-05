"""
_simulation.pyx

Copyright (c) 2021-2022 Benjamin E. Niehoff

This file is part of Lennard-Jonesium.

Lennard-Jonesium is free software: you can redistribute
it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

Lennard-Jonesium is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with Lennard-Jonesium.  If not, see
<https://www.gnu.org/licenses/>.
"""


# cimports
from libcpp.memory cimport unique_ptr, make_unique, shared_ptr
from libcpp.utility cimport move
from libcpp.string cimport string

from lennardjonesium.simulation._configuration cimport _Configuration
from lennardjonesium.simulation._simulation cimport (
    # _TextBuffer,
    _Simulation,
    # _EchoMode,
    make_simulation
)

# imports
import time

from lennardjonesium.simulation.configuration import Configuration


cdef class Simulation:
    """
    The Simulation class ...
    """
    # cdef shared_ptr[_TextBuffer] _text_buffer
    cdef unique_ptr[_Simulation] _simulation

    def __cinit__(self, configuration = None):
        if configuration is None:
            configuration = Configuration()

        if (not isinstance(configuration, Configuration)):
            raise TypeError('Initialization argument must be of type Configuration')
        
        # Build the C++ Configuration object
        # TODO: Maybe there is a way for the Python Configuration class to do this
        cdef _Configuration _configuration = _Configuration()
        
        # System settings
        _configuration.system.temperature = configuration.system.temperature
        _configuration.system.density = configuration.system.density
        _configuration.system.particle_count = configuration.system.particle_count
        _configuration.system.random_seed = configuration.system.random_seed
        _configuration.system.cutoff_distance = configuration.system.cutoff_distance
        _configuration.system.time_delta = configuration.system.time_delta

        # Equilibration settings
        _configuration.equilibration.name = bytes(configuration.equilibration.name, 'utf-8')
        _configuration.equilibration.tolerance = configuration.equilibration.tolerance
        _configuration.equilibration.sample_size = configuration.equilibration.sample_size
        _configuration.equilibration.adjustment_interval = \
            configuration.equilibration.adjustment_interval
        _configuration.equilibration.steady_state_time = \
            configuration.equilibration.steady_state_time
        _configuration.equilibration.timeout = configuration.equilibration.timeout

        # Observation settings
        _configuration.observation.name = bytes(configuration.observation.name, 'utf-8')
        _configuration.observation.tolerance = configuration.observation.tolerance
        _configuration.observation.sample_size = configuration.observation.sample_size
        _configuration.observation.observation_interval = \
            configuration.observation.observation_interval
        _configuration.observation.observation_count = configuration.observation.observation_count

        # Output files
        _configuration.filepaths.event_log = \
            bytes(configuration.filepaths.event_log, 'utf-8')
        _configuration.filepaths.thermodynamic_log = \
            bytes(configuration.filepaths.thermodynamic_log, 'utf-8')
        _configuration.filepaths.observation_log = \
            bytes(configuration.filepaths.observation_log, 'utf-8')
        _configuration.filepaths.snapshot_log = \
            bytes(configuration.filepaths.snapshot_log, 'utf-8')

        # Create the C++ Simulation object
        self._simulation = move[unique_ptr[_Simulation]](make_simulation(_configuration))
    
    # def __dealloc__(self):
    #     del self._simulation

    # def launch(self, *, echo: bool = True):
    #     if echo:
    #         self._text_buffer = self._simulation.get().launch(<_EchoMode> _buffer)
    #     else:
    #         self._simulation.get().launch(<_EchoMode> _silent)

    # def wait(self): self._simulation.get().wait()
    
    # def read(self):
    #     return str(self._text_buffer.get().read(), 'utf-8') if self._text_buffer else ""
    
    # def run(self, *, echo: bool = True, delay: float = 0.05):
    #     if echo:
    #         # We have to use the asynchronous API in order for the console output to show in a
    #         # Jupyter notebook
    #         self.launch(echo=echo)
    #         line = self.read()
    #         while line:
    #             print(line, flush=True)
    #             time.sleep(delay)
    #             line = self.read()
    #         self.wait()
    #     else:
    #         # The direct run() method uses silent mode by default
    #         self._simulation.get().run()

    def run(self, *, echo: bool = True):
        if echo:
            self._simulation.get().run(_Simulation._Echo.Console())
        else:
            self._simulation.get().run(_Simulation._Echo.Silent())

    cpdef double potential(self, double separation):
        return self._simulation.get().potential(separation)

    cpdef double virial(self, double separation):
        return self._simulation.get().virial(separation)

    cpdef double force(self, double separation):
        return self._simulation.get().force(separation)
