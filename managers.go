package lightningbug

import (
	"context"
	"sync"
	"time"

	"github.com/go-logr/logr"
)

type Manager interface {
	// A manager defines something that needs to start and stop
	Start() int
	Stop()
}

// WorldManager is a manager that moves and draws objects
type WorldManager interface {
	Manager
	Move(context.Context, Object)
	Draw(context.Context, Object)
}

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

// GameManager is a manager that runs the Game Loop
type GameManager struct {
	GameOver bool
	Objects  []Object
	Ctx      context.Context
	m        sync.Mutex

	// Managers
	lm LogManager
	wm WorldManager
	// unexported
	loopDuration time.Duration
}

func NewGameManager(lm LogManager, wm WorldManager) *GameManager {
	gm := GameManager{lm: lm, wm: wm, Objects: make([]Object, 0)}
	gm.loopDuration = 33 * time.Millisecond
	gm.Ctx = context.TODO()

	return &gm
}
func (gm *GameManager) Start() {
	gm.lm.Info("GameManager Starting ...")
	gm.GameOver = false
}

func (gm *GameManager) Stop() {
	gm.lm.Info("Goodbye Player")
	gm.GameOver = true
}

func (gm *GameManager) Run() {
	for !gm.GameOver {
		time_start := time.Now()

		for _, obj := range gm.Objects {
			obj.Update(gm.Ctx)
		}

		for _, obj := range gm.Objects {
			gm.wm.Move(gm.Ctx, obj)
		}

		for _, obj := range gm.Objects {
			gm.wm.Draw(gm.Ctx, obj)
		}

		// wait until we are ready to update
		elapsed := time.Now().Sub(time_start)
		time.Sleep(gm.loopDuration - elapsed)
	}
}

func (gm *GameManager) FrameDuration() time.Duration {
	return gm.loopDuration
}

func (gm *GameManager) Register(obj Object) {
	gm.m.Lock()
	gm.Objects = append(gm.Objects, obj)
	gm.m.Unlock()
}

type DisplayManager struct {
	Started bool
}

func (gm *DisplayManager) Start() {
	gm.Started = false
}

func (gm *DisplayManager) Stop() {
	gm.Started = true
}

type InputManager struct{}

type ResourceManager struct{}
