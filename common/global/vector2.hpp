#pragma once
#include "common_core.hpp"

#define EXPAND_VEC2(v) v.GetX(), v.GetY()

namespace Common {
	template <typename T>
	class Vector2 {
	public:
		Vector2(T x, T y)
			: m_X(x)
			, m_Y(y)
		{}

		Vector2()
			: Vector2<T>((T) 0, (T) 0)
		{}

		CLASS_GETTER_SETTER(X, T)
		CLASS_GETTER_SETTER(Y, T)

		template <typename S>
		Vector2<S> As() {
			return Vector2<S>((S) this->m_X, (S) this->m_Y);
		}

		Vector2<T>& Set(Vector2<T> vec) {
			this->m_X = vec.m_X;
			this->m_Y = vec.m_Y;
			return *this;
		}

		Vector2<T>& Set(T x, T y) {
			return this->Set(Vector2<T>(x, y));
		}

		Vector2<T>& Set(T xy) {
			return this->Set(xy, xy);
		}

		Vector2<T> Add(Vector2<T> vec) {
			return Vector2<T>(this->m_X + vec.m_X, this->m_Y + vec.m_Y);
		}

		Vector2<T> Add(T x, T y) {
			return this->Add(Vector2<T>(x, y));
		}

		Vector2<T> Add(T xy) {
			return this->Add(xy, xy);
		}

		Vector2<T> Sub(Vector2<T> vec) {
			return Vector2<T>(this->m_X - vec.m_X, this->m_Y - vec.m_Y);
		}

		Vector2<T> Sub(T x, T y) {
			return this->Sub(Vector2<T>(x, y));
		}

		Vector2<T> Sub(T xy) {
			return this->Sub(xy, xy);
		}

		Vector2<T> Mul(Vector2<T> vec) {
			return Vector2<T>(this->m_X * vec.m_X, this->m_Y * vec.m_Y);
		}

		Vector2<T> Mul(T x, T y) {
			return this->Mul(Vector2<T>(x, y));
		}

		Vector2<T> Mul(T xy) {
			return this->Mul(xy, xy);
		}

		Vector2<T> Div(Vector2<T> vec) {
			return Vector2<T>(this->m_X / vec.m_X, this->m_Y / vec.m_Y);
		}

		Vector2<T> Div(T x, T y) {
			return this->Div(Vector2<T>(x, y));
		}

		Vector2<T> Div(T xy) {
			return this->Div(xy, xy);
		}
	private:
		T m_X;
		T m_Y;
	};
}
