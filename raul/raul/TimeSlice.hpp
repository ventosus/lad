/*
  This file is part of Raul.
  Copyright 2007-2012 David Robillard <http://drobilla.net>

  Raul is free software: you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or any later version.

  Raul is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Raul.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RAUL_TIME_SLICE_HPP
#define RAUL_TIME_SLICE_HPP

#include <cassert>
#include <cmath>

#include "raul/Noncopyable.hpp"
#include "raul/TimeStamp.hpp"

namespace Raul {

/* FIXME: all the conversion here is wrong now */

/** A duration of time, with conversion between tick time and beat time.
 *
 * This is a slice along a single timeline (ie t=0 in ticks and t=0 in beats
 * are equal).  Relation to an external time base (e.g. Jack frame time) is
 * represented by frame_offset (the idea is that this holds all the information
 * necessary for passing to run() methods so they know the current state of
 * things WRT time).
 *
 * This class handles conversion between two units of time: musical
 * (beat) time, and real (tick) time.  Real time is discrete, the smallest
 * unit of time is the 'tick' (usually audio frames or MIDI ticks).  Beat time
 * is stored as a double (to be independent of any rates or timer precision).
 *
 * This caches as many values as possible to make calls less expensive, pass it
 * around by reference, not value.
 *
 * \ingroup raul
 */
class TimeSlice : public Noncopyable {
public:
	TimeSlice(uint32_t rate, uint32_t ppqn, double bpm)
		: _tick_rate(rate)
		, _beat_rate(60.0/bpm)
		, _start_ticks(Raul::TimeUnit(Raul::TimeUnit::FRAMES, rate), 0, 0)
		, _length_ticks(TimeUnit(TimeUnit::FRAMES, rate), 0, 0)
		, _start_beats(TimeUnit(TimeUnit::BEATS, ppqn), 0, 0)
		, _length_beats(TimeUnit(TimeUnit::BEATS, ppqn), 0, 0)
		, _offset_ticks(TimeUnit(TimeUnit::FRAMES, rate), 0, 0)
	{}

	/** Set the start and length of the slice.
	 *
	 * Note that external offset is not affected by this, don't forget to reset
	 * the offset each cycle!
	 */
	void set_slice(TimeStamp start, TimeDuration length) {
		assert(start.unit() == ticks_unit());
		assert(length.unit() == ticks_unit());
		_start_ticks = start;
		_length_ticks = length;
		update_beat_time();
	}

	void set_length(TimeDuration length) {
		assert(length.unit() == ticks_unit());
		_length_ticks = length;
		_length_beats = ticks_to_beats(_length_ticks);
	}

	bool contains(TimeStamp time) const {
		return (time >= start_ticks() && time < start_ticks() + length_ticks());
	}

	double tick_rate() const { return _tick_rate; }
	double beat_rate() const { return _beat_rate; }
	double bpm()       const { return 60/_beat_rate; }

	void set_tick_rate(double tick_rate) {
		_tick_rate = tick_rate;
		update_beat_time();
	}

	void set_bpm(double bpm) {
		_beat_rate = 60.0/bpm;
		update_beat_time();
	}

	inline TimeStamp beats_to_seconds(TimeStamp beats) const {
		return TimeStamp(real_unit(), beats.to_double() * 1/(double)_beat_rate);
	}

	inline TimeStamp beats_to_ticks(TimeStamp beats) const {
		return TimeStamp(ticks_unit(), beats.to_double() * (double)_beat_rate * _tick_rate);
	}

	inline TimeStamp ticks_to_seconds(TimeStamp ticks) const {
		return TimeStamp(real_unit(), ticks.ticks() * 1/(double)_tick_rate);
	}

	inline TimeStamp ticks_to_beats(TimeStamp ticks) const {
		return TimeStamp(beats_unit(), ticks.ticks() * 1/(double)_tick_rate * _beat_rate);
	}

	/** Start of current sub-cycle in ticks */
	inline TimeStamp start_ticks() const { return _start_ticks; }

	/** Length of current sub-cycle in ticks */
	inline TimeDuration length_ticks() const { return _length_ticks; }

	/** Start of current sub-cycle in beats */
	inline TimeStamp start_beats() const { return _start_beats; }

	/** Length of current sub-cycle in beats */
	inline TimeDuration length_beats() const { return _length_beats; }

	/** Set the offset between real-time and timeslice-time. */
	inline void set_offset(TimeDuration offset) { _offset_ticks = offset; }

	/** Offset relative to external (e.g Jack) time */
	inline TimeDuration offset_ticks() const { return _offset_ticks; }

	inline TimeUnit beats_unit() const { return _start_beats.unit(); }
	inline TimeUnit ticks_unit() const { return _start_ticks.unit(); }
	inline TimeUnit real_unit()  const { return TimeUnit(TimeUnit::SECONDS, 0); }

private:
	inline void update_beat_time() {
		_start_beats  = ticks_to_beats(_start_ticks);
		_length_beats = ticks_to_beats(_length_ticks);
	}

	// Rate/Tempo
	double _tick_rate; ///< Tick rate in Hz (e.g. sample rate)
	double _beat_rate; ///< Beat rate in Hz

	// Current time
	TimeStamp    _start_ticks;  ///< Current window start in ticks
	TimeDuration _length_ticks; ///< Current window length in ticks
	TimeStamp    _start_beats;  ///< Current window start in beats
	TimeDuration _length_beats; ///< Current window length in beats

	TimeDuration _offset_ticks; ///< Offset to global time
};

} // namespace Raul

#endif // RAUL_TIME_SLICE_HPP
