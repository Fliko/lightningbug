package lightningbug

import (
	"context"
	"crypto/rand"
	"io"
	"math"
	"math/big"
)

type Vector struct {
	X float64
	Y float64
}

func NewVector(x, y float64) Vector {
	return Vector{x, y}
}

func (v Vector) GetMagnitude() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func (v Vector) Scale(s float64) Vector {
	return Vector{v.X * s, v.Y * s}
}

func (v Vector) Normalize() Vector {
	mag := v.GetMagnitude()
	if mag <= 0 {
		return v
	}

	return Vector{
		X: v.X / mag,
		Y: v.Y / mag,
	}
}

func (v Vector) Add(vec Vector) Vector {
	return Vector{v.X + vec.X, v.Y + vec.Y}
}

func (v Vector) Sub(vec Vector) Vector {
	return Vector{v.X - vec.X, v.Y - vec.Y}
}

func (v Vector) Mult(vec Vector) Vector {
	return Vector{v.X * vec.X, v.Y * vec.Y}
}

func (v Vector) Div(vec Vector) Vector {
	return Vector{v.X / vec.X, v.Y / vec.Y}
}

func (v Vector) IsEqual(vec Vector) bool {
	return v.X == vec.X && v.Y == vec.Y
}

type Object interface {
	// Does the work
	Spawn(context.Context)
	Update(context.Context)
	Delete(context.Context)
	EventHandler(Event) int

	// Returns values for required data
	Kind() string
	Id() int
	Position() Vector
	Deleted() bool
}

// RandId creates a random integer
func RandId() (int, error) {
	var buf io.Reader
	id, err := rand.Int(buf, big.NewInt(1e10))
	if err != nil {
		return 0, err
	}

	return int(id.Int64()), nil
}

type ObjectList interface {
	List() []Object
	Insert(Object)
	Empty()
	DeleteMarked()

	FindId(int) *Object
	FindKind(string) []Object
}

type DefaultList []Object

func (l *DefaultList) List() []Object {
	return *l
}

func (l *DefaultList) Insert(o Object) {
	*l = append(*l, o)
}

func (l *DefaultList) Empty() {
	*l = make(DefaultList, 0)
}

func (l *DefaultList) DeleteMarked() {
	parts := make(DefaultList, 0)
	last_slice := 0
	for i, o := range *l {
		if o.Deleted() {
			parts = append(parts, (*l)[last_slice:i]...)
			last_slice = i + 1
		}
	}

	if len(parts) > 0 {
		*l = parts
	}
}

func (l *DefaultList) FindId(id int) *Object {
	for _, o := range *l {
		if o.Id() == id {
			return &o
		}
	}
	return nil
}

func (l *DefaultList) FindKind(k string) []Object {
	out := make([]Object, 0)
	for _, o := range *l {
		if o.Kind() == k {
			out = append(out, o)
		}
	}

	return out
}

func (l *DefaultList) FindDeleted(b bool) []Object {
	out := make([]Object, 0)
	for _, o := range *l {
		if o.Deleted() == b {
			out = append(out, o)
		}
	}

	return out
}
