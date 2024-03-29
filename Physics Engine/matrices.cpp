#include "Compare.h"
#include "matrices.h"
#include <cmath>
#include <cfloat>
#include <iostream>

bool operator==(const mat2& l, const mat2& r) {
	for (int i = 0; i < /* 2 * 2 = */4; ++i) {
		if (!CMP(l.asArray[i], r.asArray[i])) {
			return false;
		}
	}
	return true;
}

bool operator==(const mat3& l, const mat3& r) {
	for (int i = 0; i < /* 3 * 3 = */ 9; ++i) {
		if (!CMP(l.asArray[i], r.asArray[i])) {
			return false;
		}
	}
	return true;
}
bool operator==(const physmat4& l, const physmat4& r) {
	for (int i = 0; i < /* 4 * 4 = */ 16; ++i) {
		if (!CMP(l.asArray[i], r.asArray[i])) {
			return false;
		}
	}
	return true;
}

bool operator!=(const mat2& l, const mat2& r) {
	return !(l == r);
}

bool operator!=(const mat3& l, const mat3& r) {
	return !(l == r);
}

bool operator!=(const physmat4& l, const physmat4& r) {
	return !(l == r);
}

std::ostream& operator<<(std::ostream& os, const mat2& m) {
	os << m._11 << ", " << m._12 << "\n";
	os << m._21 << ", " << m._22;
	return os;
}

std::ostream& operator<<(std::ostream& os, const mat3& m) {
	os << m._11 << ", " << m._12 << ", " << m._13 << "\n";
	os << m._21 << ", " << m._22 << ", " << m._23 << "\n";
	os << m._31 << ", " << m._32 << ", " << m._33;
	return os;
}

std::ostream& operator<<(std::ostream& os, const physmat4& m) {
	os << m._11 << ", " << m._12 << ", " << m._13 << ", " << m._14 << "\n";
	os << m._21 << ", " << m._22 << ", " << m._23 << ", " << m._24 << "\n";
	os << m._31 << ", " << m._32 << ", " << m._33 << ", " << m._34 << "\n";
	os << m._41 << ", " << m._42 << ", " << m._43 << ", " << m._44;
	return os;
}

mat3 FastInverse(const mat3& mat) {
	return Transpose(mat);
}

physmat4 FastInverse(const physmat4& mat) {

	physmat4 inverse = Transpose(mat);
	inverse._41 = inverse._14 = 0.0f;
	inverse._42 = inverse._24 = 0.0f;
	inverse._43 = inverse._34 = 0.0f;

	physvec3 right = physvec3(mat._11, mat._12, mat._13);
	physvec3 up = physvec3(mat._21, mat._22, mat._23);
	physvec3 forward = physvec3(mat._31, mat._32, mat._33);
	physvec3 position = physvec3(mat._41, mat._42, mat._43);

	inverse._41 = -Dot(right, position);
	inverse._42 = -Dot(up, position);
	inverse._43 = -Dot(forward, position);

	return inverse;
}

void Transpose(const float *srcMat, float *dstMat, int srcRows, int srcCols) {
	for (int i = 0; i < srcRows * srcCols; i++) {
		int row = i / srcRows;
		int col = i % srcRows;
		dstMat[i] = srcMat[srcCols * col + row];
	}
}

mat2 Transpose(const mat2& matrix) {
	mat2 result;
	Transpose(matrix.asArray, result.asArray, 2, 2);
	return result;
}

mat3 Transpose(const mat3& matrix) {
	mat3 result;
	Transpose(matrix.asArray, result.asArray, 3, 3);
	return result;
}

physmat4 Transpose(const physmat4& matrix) {
	physmat4 result;
	Transpose(matrix.asArray, result.asArray, 4, 4);
	return result;
}

mat2 operator*(const mat2& matrix, float scalar) {
	mat2 result;
	for (int i = 0; i < 4; ++i) {
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

mat3 operator*(const mat3& matrix, float scalar) {
	mat3 result;
	for (int i = 0; i < 9; ++i) {
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

physmat4 operator*(const physmat4& matrix, float scalar) {
	physmat4 result;
	for (int i = 0; i < 16; ++i) {
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

bool Multiply(float* out, const float* matA, int aRows, int aCols, const float* matB, int bRows, int bCols) {
	if (aCols != bRows) {
		return false;
	}

	for (int i = 0; i < aRows; ++i) {
		for (int j = 0; j < bCols; ++j) {
			out[bCols * i + j] = 0.0f;
			for (int k = 0; k < bRows; ++k) {
				out[bCols * i + j] += matA[aCols * i + k] * matB[bCols * k + j];
			}
		}
	}

	return true;
}

mat2 operator*(const mat2& matrixA, const mat2& matrixB) {
	mat2 result;
	Multiply(result.asArray, matrixA.asArray, 2, 2, matrixB.asArray, 2, 2);
	return result;
}

mat3 operator*(const mat3& matrixA, const mat3& matrixB) {
	mat3 result;
	Multiply(result.asArray, matrixA.asArray, 3, 3, matrixB.asArray, 3, 3);
	return result;
}

physmat4 operator*(const physmat4& matrixA, const physmat4& matrixB) {
	physmat4 result;
	Multiply(result.asArray, matrixA.asArray, 4, 4, matrixB.asArray, 4, 4);
	return result;
}

float Determinant(const mat2& matrix) {
	return matrix._11 * matrix._22 - matrix._12 * matrix._21;
}

mat2 Cut(const mat3& mat, int row, int col) {
	mat2 result;
	int index = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == row || j == col) {
				continue;
			}
			result.asArray[index++] = mat.asArray[3 * i + j];
		}
	}

	return result;
}

mat3 Cut(const physmat4& mat, int row, int col) {
	mat3 result;
	int index = 0;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == row || j == col) {
				continue;
			}
			result.asArray[index++] = mat.asArray[4 * i + j];
		}
	}

	return result;
}

mat3 Minor(const mat3& mat) {
	mat3 result;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result[i][j] = Determinant(Cut(mat, i, j));
		}
	}

	return result;
}

mat2 Minor(const mat2& mat) {
	return mat2(
		mat._22, mat._21,
		mat._12, mat._11
	);
}

void Cofactor(float* out, const float* minor, int rows, int cols) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			out[cols * j + i] = minor[cols * j + i] * powf(-1.0f, i + j);
		}
	}
}

mat2 Cofactor(const mat2& mat) {
	mat2 result;
	Cofactor(result.asArray, Minor(mat).asArray, 2, 2);
	return result;
}

mat3 Cofactor(const mat3& mat) {
	mat3 result;
	Cofactor(result.asArray, Minor(mat).asArray, 3, 3);
	return result;
}

float Determinant(const mat3& mat) {
	float result = 0.0f;

	/*float A = mat.asArray[3 * 0 + 0] * Determinant(Cut(mat, 0, 0));
	float B = mat.asArray[3 * 0 + 1] * Determinant(Cut(mat, 0, 1));
	float C = mat.asArray[3 * 0 + 2] * Determinant(Cut(mat, 0, 2));
	result = A - B + C;*/

	/*for (int j = 0; j < 3; ++j) {
	result += mat.asArray[3 * 0 + j] * Determinant(Cut(mat, 0, j)) * powf(-1, 0 + j);
	}*/

	mat3 cofactor = Cofactor(mat);
	for (int j = 0; j < 3; ++j) {
		result += mat.asArray[3 * 0 + j] * cofactor[0][j];
	}

	return result;
}

physmat4 Minor(const physmat4& mat) {
	physmat4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = Determinant(Cut(mat, i, j));
		}
	}

	return result;
}

physmat4 Cofactor(const physmat4& mat) {
	physmat4 result;
	Cofactor(result.asArray, Minor(mat).asArray, 4, 4);
	return result;
}

float Determinant(const physmat4& mat) {
	float result = 0.0f;

	physmat4 cofactor = Cofactor(mat);
	for (int j = 0; j < 4; ++j) {
		result += mat.asArray[4 * 0 + j] * cofactor[0][j];
	}

	return result;
}

mat2 Adjugate(const mat2& mat) {
	return Transpose(Cofactor(mat));
}

mat3 Adjugate(const mat3& mat) {
	return Transpose(Cofactor(mat));
}

physmat4 Adjugate(const physmat4& mat) {
	return Transpose(Cofactor(mat));
}

mat2 Inverse(const mat2& mat) {
	float det = Determinant(mat);
	if (CMP(det, 0.0f)) { return mat2(); }
	return Adjugate(mat) * (1.0f / det);

	/*float det = mat._11 * mat._22 - mat._12 * mat._21;
	if (CMP(det, 0.0f)) {
	return mat2();
	}
	float i_det = 1.0f / det;
	mat2 result;
	result._11 =  mat._22 * i_det;
	result._12 = -mat._12 * i_det;
	result._21 = -mat._21 * i_det;
	result._22 =  mat._11 * i_det;
	return result;*/
}

mat3 Inverse(const mat3& mat) {
	float det = Determinant(mat);
	if (CMP(det, 0.0f)) { return mat3(); }
	return Adjugate(mat) * (1.0f / det);
}

physmat4 Inverse(const physmat4& m) {
	/*float det = Determinant(m);
	if (CMP(det, 0.0f)) { return mat4(); }
	return Adjugate(m) * (1.0f / det);*/

	// The code below is the expanded form of the above equation.
	// This optimization avoids loops and function calls

	float det
		= m._11 * m._22 * m._33 * m._44 + m._11 * m._23 * m._34 * m._42 + m._11 * m._24 * m._32 * m._43
		+ m._12 * m._21 * m._34 * m._43 + m._12 * m._23 * m._31 * m._44 + m._12 * m._24 * m._33 * m._41
		+ m._13 * m._21 * m._32 * m._44 + m._13 * m._22 * m._34 * m._41 + m._13 * m._24 * m._31 * m._42
		+ m._14 * m._21 * m._33 * m._42 + m._14 * m._22 * m._31 * m._43 + m._14 * m._23 * m._32 * m._41
		- m._11 * m._22 * m._34 * m._43 - m._11 * m._23 * m._32 * m._44 - m._11 * m._24 * m._33 * m._42
		- m._12 * m._21 * m._33 * m._44 - m._12 * m._23 * m._34 * m._41 - m._12 * m._24 * m._31 * m._43
		- m._13 * m._21 * m._34 * m._42 - m._13 * m._22 * m._31 * m._44 - m._13 * m._24 * m._32 * m._41
		- m._14 * m._21 * m._32 * m._43 - m._14 * m._22 * m._33 * m._41 - m._14 * m._23 * m._31 * m._42;

	if (CMP(det, 0.0f)) {
		return physmat4();
	}
	float i_det = 1.0f / det;

	physmat4 result;
	result._11 = (m._22 * m._33 * m._44 + m._23 * m._34 * m._42 + m._24 * m._32 * m._43 - m._22 * m._34 * m._43 - m._23 * m._32 * m._44 - m._24 * m._33 * m._42) * i_det;
	result._12 = (m._12 * m._34 * m._43 + m._13 * m._32 * m._44 + m._14 * m._33 * m._42 - m._12 * m._33 * m._44 - m._13 * m._34 * m._42 - m._14 * m._32 * m._43) * i_det;
	result._13 = (m._12 * m._23 * m._44 + m._13 * m._24 * m._42 + m._14 * m._22 * m._43 - m._12 * m._24 * m._43 - m._13 * m._22 * m._44 - m._14 * m._23 * m._42) * i_det;
	result._14 = (m._12 * m._24 * m._33 + m._13 * m._22 * m._34 + m._14 * m._23 * m._32 - m._12 * m._23 * m._34 - m._13 * m._24 * m._32 - m._14 * m._22 * m._33) * i_det;
	result._21 = (m._21 * m._34 * m._43 + m._23 * m._31 * m._44 + m._24 * m._33 * m._41 - m._21 * m._33 * m._44 - m._23 * m._34 * m._41 - m._24 * m._31 * m._43) * i_det;
	result._22 = (m._11 * m._33 * m._44 + m._13 * m._34 * m._41 + m._14 * m._31 * m._43 - m._11 * m._34 * m._43 - m._13 * m._31 * m._44 - m._14 * m._33 * m._41) * i_det;
	result._23 = (m._11 * m._24 * m._43 + m._13 * m._21 * m._44 + m._14 * m._23 * m._41 - m._11 * m._23 * m._44 - m._13 * m._24 * m._41 - m._14 * m._21 * m._43) * i_det;
	result._24 = (m._11 * m._23 * m._34 + m._13 * m._24 * m._31 + m._14 * m._21 * m._33 - m._11 * m._24 * m._33 - m._13 * m._21 * m._34 - m._14 * m._23 * m._31) * i_det;
	result._31 = (m._21 * m._32 * m._44 + m._22 * m._34 * m._41 + m._24 * m._31 * m._42 - m._21 * m._34 * m._42 - m._22 * m._31 * m._44 - m._24 * m._32 * m._41) * i_det;
	result._32 = (m._11 * m._34 * m._42 + m._12 * m._31 * m._44 + m._14 * m._32 * m._41 - m._11 * m._32 * m._44 - m._12 * m._34 * m._41 - m._14 * m._31 * m._42) * i_det;
	result._33 = (m._11 * m._22 * m._44 + m._12 * m._24 * m._41 + m._14 * m._21 * m._42 - m._11 * m._24 * m._42 - m._12 * m._21 * m._44 - m._14 * m._22 * m._41) * i_det;
	result._34 = (m._11 * m._24 * m._32 + m._12 * m._21 * m._34 + m._14 * m._22 * m._31 - m._11 * m._22 * m._34 - m._12 * m._24 * m._31 - m._14 * m._21 * m._32) * i_det;
	result._41 = (m._21 * m._33 * m._42 + m._22 * m._31 * m._43 + m._23 * m._32 * m._41 - m._21 * m._32 * m._43 - m._22 * m._33 * m._41 - m._23 * m._31 * m._42) * i_det;
	result._42 = (m._11 * m._32 * m._43 + m._12 * m._33 * m._41 + m._13 * m._31 * m._42 - m._11 * m._33 * m._42 - m._12 * m._31 * m._43 - m._13 * m._32 * m._41) * i_det;
	result._43 = (m._11 * m._23 * m._42 + m._12 * m._21 * m._43 + m._13 * m._22 * m._41 - m._11 * m._22 * m._43 - m._12 * m._23 * m._41 - m._13 * m._21 * m._42) * i_det;
	result._44 = (m._11 * m._22 * m._33 + m._12 * m._23 * m._31 + m._13 * m._21 * m._32 - m._11 * m._23 * m._32 - m._12 * m._21 * m._33 - m._13 * m._22 * m._31) * i_det;

	/*if (result * m != mat4()) {
	std::cout << "ERROR! Expecting matrix x inverse to equal identity!\n";
	}*/

	return result;
}

physmat4 ToColumnMajor(const physmat4& mat) {
	return Transpose(mat);
}

mat3 ToColumnMajor(const mat3& mat) {
	return Transpose(mat);
}

physmat4 FromColumnMajor(const physmat4& mat) {
	return Transpose(mat);
}

mat3 FromColumnMajor(const mat3& mat) {
	return Transpose(mat);
}

physmat4 FromColumnMajor(const float* mat) {
	return Transpose(physmat4(
		mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]
	));
}

physmat4 Translation(float x, float y, float z) {
	return physmat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	);
}
physmat4 Translation(const physvec3& pos) {
	return physmat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
}

physmat4 Translate(float x, float y, float z) {
	return physmat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	);
}

physmat4 Translate(const physvec3& pos) {
	return physmat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
}

physmat4 FromMat3(const mat3& mat) {
	physmat4 result;

	result._11 = mat._11;
	result._12 = mat._12;
	result._13 = mat._13;

	result._21 = mat._21;
	result._22 = mat._22;
	result._23 = mat._23;

	result._31 = mat._31;
	result._32 = mat._32;
	result._33 = mat._33;

	return result;
}

physvec3 GetTranslation(const physmat4& mat) {
	return physvec3(mat._41, mat._42, mat._43);
}

physmat4 Scale(float x, float y, float z) {
	return physmat4(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

physmat4 Scale(const physvec3& vec) {
	return physmat4(
		vec.x, 0.0f, 0.0f, 0.0f,
		0.0f, vec.y, 0.0f, 0.0f,
		0.0f, 0.0f, vec.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

physvec3 GetScale(const physmat4& mat) {
	return physvec3(mat._11, mat._22, mat._33);
}

physmat4 Rotation(float pitch, float yaw, float roll) {
	return  ZRotation(roll) * XRotation(pitch) * YRotation(yaw);
}

mat3 Rotation3x3(float pitch, float yaw, float roll) {
	return ZRotation3x3(roll) * XRotation3x3(pitch) * YRotation3x3(yaw);
}

mat2 Rotation2x2(float angle) {
	return mat2(
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle)
	);
}

physmat4 YawPitchRoll(float yaw, float pitch, float roll) {
	yaw = DEG2RAD(yaw);
	pitch = DEG2RAD(pitch);
	roll = DEG2RAD(roll);

	physmat4 out; // z * x * y
	out._11 = (cosf(roll) * cosf(yaw)) + (sinf(roll) * sinf(pitch) * sinf(yaw));
	out._12 = (sinf(roll) * cosf(pitch));
	out._13 = (cosf(roll) * -sinf(yaw)) + (sinf(roll) * sinf(pitch) * cosf(yaw));
	out._21 = (-sinf(roll) * cosf(yaw)) + (cosf(roll) * sinf(pitch) * sinf(yaw));
	out._22 = (cosf(roll) * cosf(pitch));
	out._23 = (sinf(roll) * sinf(yaw)) + (cosf(roll) * sinf(pitch) * cosf(yaw));
	out._31 = (cosf(pitch) * sinf(yaw));
	out._32 = -sinf(pitch);
	out._33 = (cosf(pitch) * cosf(yaw));
	out._44 = 1;
	return out;
}

physmat4 XRotation(float angle) {
	angle = DEG2RAD(angle);
	return physmat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(angle), sinf(angle), 0.0f,
		0.0f, -sinf(angle), cosf(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

mat3 XRotation3x3(float angle) {
	angle = DEG2RAD(angle);
	return mat3(
		1.0f, 0.0f, 0.0f,
		0.0f, cosf(angle), sinf(angle),
		0.0f, -sinf(angle), cosf(angle)
	);
}

physmat4 YRotation(float angle) {
	angle = DEG2RAD(angle);
	return physmat4(
		cosf(angle), 0.0f, -sinf(angle), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(angle), 0.0f, cosf(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

mat3 YRotation3x3(float angle) {
	angle = DEG2RAD(angle);
	return mat3(
		cosf(angle), 0.0f, -sinf(angle),
		0.0f, 1.0f, 0.0f,
		sinf(angle), 0.0f, cosf(angle)
	);
}

physmat4 ZRotation(float angle) {
	angle = DEG2RAD(angle);
	return physmat4(
		cosf(angle), sinf(angle), 0.0f, 0.0f,
		-sinf(angle), cosf(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

mat3 ZRotation3x3(float angle) {
	angle = DEG2RAD(angle);
	return mat3(
		cosf(angle), sinf(angle), 0.0f,
		-sinf(angle), cosf(angle), 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

physmat4 Orthogonalize(const physmat4& mat) {
	physvec3 xAxis(mat._11, mat._12, mat._13);
	physvec3 yAxis(mat._21, mat._22, mat._23);
	physvec3 zAxis = Cross(xAxis, yAxis);

	xAxis = Cross(yAxis, zAxis);
	yAxis = Cross(zAxis, xAxis);
	zAxis = Cross(xAxis, yAxis);

	return physmat4(
		xAxis.x, xAxis.y, xAxis.z, mat._14,
		yAxis.x, yAxis.y, yAxis.z, mat._24,
		zAxis.x, zAxis.y, zAxis.z, mat._34,
		mat._41, mat._42, mat._43, mat._44
	);
}

mat3 Orthogonalize(const mat3& mat) {
	physvec3 xAxis(mat._11, mat._12, mat._13);
	physvec3 yAxis(mat._21, mat._22, mat._23);
	physvec3 zAxis = Cross(xAxis, yAxis);

	xAxis = Cross(yAxis, zAxis);
	yAxis = Cross(zAxis, xAxis);
	zAxis = Cross(xAxis, yAxis);

	return mat3(
		xAxis.x, xAxis.y, xAxis.z,
		yAxis.x, yAxis.y, yAxis.z,
		zAxis.x, zAxis.y, zAxis.z
	);
}

physmat4 AxisAngle(const physvec3& axis, float angle) {
	angle = DEG2RAD(angle);
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - cosf(angle);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	if (!CMP(MagnitudeSq(axis), 1.0f)) {
		float inv_len = 1.0f / Magnitude(axis);
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
	}

	return physmat4(
		t * (x * x) + c, t * x * y + s * z, t * x * z - s * y, 0.0f,
		t * x * y - s * z, t * (y * y) + c, t * y * z + s * x, 0.0f,
		t * x * z + s * y, t * y * z - s * x, t * (z * z) + c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

mat3 AxisAngle3x3(const physvec3& axis, float angle) {
	angle = DEG2RAD(angle);
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - cosf(angle);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	if (!CMP(MagnitudeSq(axis), 1.0f)) {
		float inv_len = 1.0f / Magnitude(axis);
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
	}

	return mat3(
		t * (x * x) + c, t * x * y + s * z, t * x * z - s * y,
		t * x * y - s * z, t * (y * y) + c, t * y * z + s * x,
		t * x * z + s * y, t * y * z - s * x, t * (z * z) + c
	);
}

physvec3 MultiplyPoint(const physvec3& vec, const physmat4& mat) {
	physvec3 result;
	result.x = vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31 + 1.0f * mat._41;
	result.y = vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32 + 1.0f * mat._42;
	result.z = vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33 + 1.0f * mat._43;
	return result;
}

physvec3 MultiplyVector(const physvec3& vec, const physmat4& mat) {
	physvec3 result;
	result.x = vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31 + 0.0f * mat._41;
	result.y = vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32 + 0.0f * mat._42;
	result.z = vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33 + 0.0f * mat._43;
	return result;
}

physvec3 MultiplyVector(const physvec3& vec, const mat3& mat) {
	physvec3 result;
	result.x = Dot(vec, physvec3{ mat._11, mat._21, mat._31 });
	result.y = Dot(vec, physvec3{ mat._12, mat._22, mat._32 });
	result.z = Dot(vec, physvec3{ mat._13, mat._23, mat._33 });
	return result;
}

physmat4 Transform(const physvec3& scale, const physvec3& eulerRotation, const physvec3& translate) {
	return Scale(scale) *
		Rotation(eulerRotation.x, eulerRotation.y, eulerRotation.z) *
		Translation(translate);
}

physmat4 Transform(const physvec3& scale, const physvec3& rotationAxis, float rotationAngle, const physvec3& translate) {
	return Scale(scale) *
		AxisAngle(rotationAxis, rotationAngle) *
		Translation(translate);
}

physmat4 LookAt(const physvec3& position, const physvec3& target, const physvec3& up) {
	physvec3 forward = Normalized(target - position);
	physvec3 right = Normalized(Cross(up, forward));
	physvec3 newUp = Cross(forward, right);

	return physmat4(
		right.x, newUp.x, forward.x, 0.0f,
		right.y, newUp.y, forward.y, 0.0f,
		right.z, newUp.z, forward.z, 0.0f,
		-Dot(right, position), -Dot(newUp, position), -Dot(forward, position), 1.0f
	);
}

physmat4 Projection(float fov, float aspect, float zNear, float zFar) {
	float tanHalfFov = tanf(DEG2RAD((fov * 0.5f)));
	float fovY = 1.0f / tanHalfFov; // cot(fov/2)
	float fovX = fovY / aspect; // cot(fov/2) / aspect

	physmat4 result;

	result._11 = fovX;
	result._22 = fovY;
	result._33 = zFar / (zFar - zNear); // far / range
	result._34 = 1.0f;
	result._43 = -zNear * result._33; // - near * (far / range)
	result._44 = 0.0f;

	return result;
}

physmat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
	float _11 = 2.0f / (right - left);
	float _22 = 2.0f / (top - bottom);
	float _33 = 1.0f / (zFar - zNear);
	float _41 = (left + right) / (left - right);
	float _42 = (top + bottom) / (bottom - top);
	float _43 = (zNear) / (zNear - zFar);

	return physmat4(
		_11, 0.0f, 0.0f, 0.0f,
		0.0f, _22, 0.0f, 0.0f,
		0.0f, 0.0f, _33, 0.0f,
		_41, _42, _43, 1.0f
	);
}

physvec3 Decompose(const mat3& rot1) {
	mat3 rot = Transpose(rot1);

	float sy = sqrt(rot._11 * rot._11 + rot._21 * rot._21);

	bool singular = sy < 1e-6;

	float x, y, z;
	if (!singular) {
		x = atan2(rot._32, rot._33);
		y = atan2(-rot._31, sy);
		z = atan2(rot._21, rot._11);
	}
	else {
		x = atan2(-rot._23, rot._22);
		y = atan2(-rot._31, sy);
		z = 0;
	}

	return physvec3(x, y, z);
}