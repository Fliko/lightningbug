package lightningbug

import "github.com/go-logr/logr"

// LogManager is a manager that controlls logging
type LogManager struct {
	Started bool
	// When logger is not started Logger discards all messages
	// and a reference to the original logger is stored in paused
	logr.Logger
	paused logr.Logger
}

func NewLogManager(logger *logr.Logger) LogManager {
	lm := LogManager{}
	lm.Logger = logger.WithValues("from_lightningbug", true)

	return lm
}

func (lm *LogManager) Start() {
	if lm.Started {
		return
	}

	lm.Started = true
	lm.Logger = lm.paused
	lm.paused = logr.Logger{}
}
func (lm *LogManager) Stop() {
	if !lm.Started {
		return
	}

	lm.Started = false
	lm.paused = lm.Logger
	lm.Logger = logr.Discard()
}
