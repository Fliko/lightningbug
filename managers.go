package lightningbug

import (
	"time"

	"github.com/go-logr/logr"
)

type ManagerIface interface {
	Start() int
	Stop()

	// Required to Inherit Manager struct
	IsStarted() bool
	GetType() string
}

type Manager struct {
	mType     string
	isStarted bool
}

func setType(str string) func(*Manager) {
	return func(m *Manager) {
		m.mType = str
	}
}

func setStarted(is_started bool) func(*Manager) {
	return func(m *Manager) {
		m.isStarted = is_started
	}
}

func NewManager(opts ...func(*Manager)) Manager {
	m := Manager{"manager", false}

	for _, opt := range opts {
		opt(&m)
	}

	return m
}

func (m *Manager) IsStarted() bool {
	return m.isStarted
}

func (m *Manager) GetType() string {
	return m.mType
}

func (m *Manager) Start() int {
	m.isStarted = true
	return 0
}

func (m *Manager) Stop() {
	m.isStarted = false
}

type LogManager struct {
	Manager

	// When logger is not started Logger discards all messages
	// and a reference to the original logger is stored in paused
	logr.Logger
	paused logr.Logger
}

func NewLogManager(logger *logr.Logger) LogManager {
	lm := LogManager{}
	lm.Manager = NewManager(setType("LogManager"), setStarted(true))
	lm.Logger = logger.WithValues("from_lightningbug", true)

	return lm
}

func (lm *LogManager) Start() {
	if lm.IsStarted() {
		return
	}

	lm.isStarted = true
	lm.Logger = lm.paused
	lm.paused = logr.Logger{}
}
func (lm *LogManager) Stop() {
	lm.isStarted = false
	lm.paused = lm.Logger
	lm.Logger = logr.Discard()
}

type GameManager struct {
	Manager
	GameOver bool

	// unexported
	loopDuration time.Duration
}

func NewGameManager() *GameManager {
	gm := GameManager{}
	gm.Manager = NewManager(setType("GameManager"))
	gm.loopDuration = 33 * time.Millisecond

	return &gm
}

func (gm *GameManager) Run() {
	for !gm.GameOver {
		time_start := time.Now()

		// wait until we are ready to update
		elapsed := time.Now().Sub(time_start)
		time.Sleep(gm.loopDuration - elapsed)
	}
}

func (gm *GameManager) Stop() {
	gm.GameOver = true
}

func (gm *GameManager) FrameDuration() time.Duration {
	return gm.loopDuration
}

type DisplayManager struct{}

type InputManager struct{}

type ResourceManager struct{}

type WorldManager struct{}
