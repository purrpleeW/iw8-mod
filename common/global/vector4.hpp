#pragma once
#include "common_core.hpp"

#define EXPAND_VEC4(v) v.GetX(), v.GetY(), v.GetZ(), v.GetW()

namespace Common {
	template <typename T>
	class Vector4 {
	public:
		Vector4(T x, T y, T z, T w)
			: m_X(x)
			, m_Y(y)
			, m_Z(z)
			, m_W(w)
		{}

		Vector4()
			: Vector4<T>((T) 0, (T) 0, (T) 0, (T) 0)
		{}

		CLASS_GETTER_SETTER(X, T)
		CLASS_GETTER_SETTER(Y, T)
		CLASS_GETTER_SETTER(Z, T)
		CLASS_GETTER_SETTER(W, T)

		template <typename S>
		Vector4<S> As() {
			return Vector4<S>((S) this->m_X, (S) this->m_Y, (S) this->m_Z, (S) this->m_W);
		}

		Vector4<T>& Set(Vector4<T> vec) {
			this->m_X = vec.m_X;
			this->m_Y = vec.m_Y;
			this->m_Z = vec.m_Z;
			this->m_W = vec.m_W;
			return *this;
		}

		Vector4<T>& Set(T x, T y, T z, T w) {
			return this->Set(Vector4<T>(x, y, z, w));
		}

		Vector4<T>& Set(T xyzw) {
			return this->Set(xyzw, xyzw, xyzw, xyzw);
		}

		Vector4<T> Add(Vector4<T> vec) {
			return Vector4<T>(this->m_X + vec.m_X, this->m_Y + vec.m_Y, this->m_Z + vec.m_Z, this->m_W + vec.m_W);
		}

		Vector4<T> Add(T x, T y, T z, T w) {
			return this->Add(Vector4<T>(x, y, z, w));
		}

		Vector4<T> Add(T xyzw) {
			return this->Add(xyzw, xyzw, xyzw, xyzw);
		}

		Vector4<T> Sub(Vector4<T> vec) {
			return Vector4<T>(this->m_X - vec.m_X, this->m_Y - vec.m_Y, this->m_Z - vec.m_Z, this->m_W - vec.m_W);
		}

		Vector4<T> Sub(T x, T y, T z, T w) {
			return this->Sub(Vector4<T>(x, y, z, w));
		}

		Vector4<T> Sub(T xyzw) {
			return this->Sub(xyzw, xyzw, xyzw, xyzw);
		}

		Vector4<T> Mul(Vector4<T> vec) {
			return Vector4<T>(this->m_X * vec.m_X, this->m_Y * vec.m_Y, this->m_Z * vec.m_Z, this->m_W * vec.m_W);
		}

		Vector4<T> Mul(T x, T y, T z, T w) {
			return this->Mul(Vector4<T>(x, y, z, w));
		}

		Vector4<T> Mul(T xyzw) {
			return this->Mul(xyzw, xyzw, xyzw, xyzw);
		}

		Vector4<T> Div(Vector4<T> vec) {
			return Vector4<T>(this->m_X / vec.m_X, this->m_Y / vec.m_Y, this->m_Z / vec.m_Z, this->m_W / vec.m_W);
		}

		Vector4<T> Div(T x, T y, T z, T w) {
			return this->Div(Vector4<T>(x, y, z, w));
		}

		Vector4<T> Div(T xyzw) {
			return this->Div(xyzw, xyzw, xyzw, xyzw);
		}
	private:
		T m_X;
		T m_Y;
		T m_Z;
		T m_W;
	};
}
