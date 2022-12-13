#pragma once
class CGLRenderer
{
private:
	HGLRC m_hrc;
	float alpha = 0.0f;
	float beta = 0.7f;
	float rotAngle = 0;
public:
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void ChangeAngle(float angle, bool a);
	void ChangeLook(float alpha, float beta);
	void SetRot(float angl);
};

