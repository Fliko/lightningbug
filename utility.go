package lightningbug

import "math"

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

type Object struct {
	Id       int
	ObjType  string
	Position Vector
}
