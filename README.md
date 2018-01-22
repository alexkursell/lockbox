# Whistle-Activated Lockbox

This Arduino project is for a whistle activated lockbox. An Arduino is connected to a microphone, display LED and locking servo and placed in a lockbox. When a specific tune is whistled to it (the default being the first few notes of Twinkle Twinkle Little Star), the box toggles the lock.

Uses the ArduinoFHT library to perform real-time frequency analysis and a version of the Needleman-Wunsch algorithm to check that the input matches the special tune. See demo.mov for video of the box in action.
