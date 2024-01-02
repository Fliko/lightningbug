package lightningbug

import (
	"context"
	"sync"
	"time"
)

// GameManager is a manager that runs the Game Loop
type GameManager struct {
	GameOver bool
	Step     Stepper
	Ctx      context.Context
	m        sync.Mutex

	// Managers
	lm LogManager
	wm WorldManager
	// unexported
	loopDuration time.Duration
}

func NewGameManager(lm LogManager, wm WorldManager) *GameManager {
	gm := GameManager{lm: lm, wm: wm, Step: Stepper{Evt: NewEvt(STEP_EVENT)}}
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

		gm.wm.Update(gm.Ctx)

		list := gm.wm.Objects.List()
		for _, obj := range list {
			gm.Step.Count++
			obj.EventHandler(gm.Step)
		}
		for _, obj := range gm.wm.Objects.List() {
			gm.wm.Move(gm.Ctx, obj)
		}

		for _, obj := range gm.wm.Objects.List() {
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
	gm.wm.Objects.Insert(obj)
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
