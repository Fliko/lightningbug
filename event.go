package lightningbug

type Event interface {
	Name() string
}

type Evt struct {
	EventName string
}

func NewEvt(name string) Evt {
	return Evt{EventName: name}
}

func (e Evt) Name() string {
	return e.EventName
}

const STEP_EVENT = "STEP_EVENT"

type Stepper struct {
	Evt
	Count int
}
