package lightningbug

import (
	"context"

	"github.com/pkg/errors"
)

// WorldManager is a manager that moves and draws objects
type WorldManager struct {
	Started bool
	Objects ObjectList
}

func NewWorldManager(t ObjectList) (*WorldManager, error) {
	if t == nil {
		return nil, errors.New("objectList must not be nil")
	}

	return &WorldManager{
		Started: true,
		Objects: t,
	}, nil
}

func (wm *WorldManager) Start() int {
	if wm.Started {
		return 1
	}
	wm.Started = true
	return 1
}

func (wm *WorldManager) Stop() {
	if !wm.Started {
		return
	}
	wm.Started = false
	wm.Objects.Empty()
}

func (wm *WorldManager) Update(ctx context.Context) {
	l := wm.Objects.List()
	wm.Objects.Empty()
	for _, o := range l {
		if !o.Deleted() {
			o.Update(ctx)
			wm.Objects.Insert(o)
		} else {
			o.Delete(ctx)
		}
	}
}

func (wm *WorldManager) Move(ctx context.Context, o Object) {}
func (wm *WorldManager) Draw(ctx context.Context, o Object) {}
