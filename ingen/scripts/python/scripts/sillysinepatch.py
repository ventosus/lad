#!/usr/bin/python
#
# Python bindings for Om
# Copyright 2005 Leonard Ritter
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

import ingen
import os,time,sys

def main(om):
	om.setEnvironment(ingen.Environment())
	om.engine.activate()
	om.engine.load_plugins()
	om.engine.register_client(om.getAddressAsString())
	om.request.all_objects(om.getAddressAsString())
	om.synth.create_patch("/silly_sine", 1)
	om.synth.create_node("/silly_sine/output", "Internal", "", "audio_output", 0)
	om.synth.create_node("/silly_sine/sine", "LADSPA", "cmt.so", "sine_fcac", 0)
	om.synth.set_property("/silly_sine/sine/Frequency", "http://drobilla.net/ns/ingen#value", 440.0)
	om.synth.set_property("/silly_sine/sine/Amplitude", "http://drobilla.net/ns/ingen#value", 1.0)
	om.synth.connect("/silly_sine/sine/Output", "/silly_sine/output/out")
	om.synth.enable_patch("/silly_sine")
	om.engine.enable()
	om.engine.unregister_client(om.getAddressAsString())
	os._exit(0)

if __name__ == "__main__":
	ingen.startClient(main)
