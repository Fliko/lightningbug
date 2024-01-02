package lightningbug

type Manager interface {
	// A manager defines something that needs to start and stop
	Start() int
	Stop()
}

type InputManager struct{}

type ResourceManager struct{}
