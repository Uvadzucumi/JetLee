
#include "camera.h"

using namespace MyOGL;

#include <iostream>

void CCamera::Apply(){
    Render->LookAt(  m_position.x,  m_position.y,   m_position.z,
                     m_view.x,      m_view.y,       m_view.z,
                     m_up.x,        m_up.y,         m_up.z);
}

void CCamera::set(float positionX, float positionY, float positionZ,
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ){
    this->setPosition(positionX,positionY,positionZ);
    this->setView(viewX,viewY,viewZ);
    this->setUp(upVectorX,upVectorY,upVectorZ);
}

void CCamera::Debug(){
    std::cout << "--------- Camera Debug ----------" << std::endl;
    std::cout << "Position: " << m_position << std::endl;
    std::cout << "View: " << m_view << std::endl;
    std::cout << "Up: " << m_up << std::endl;
    std::cout << "Direction: " << direction << std::endl;
    std::cout << "Shift: " << m_strafe << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

void CCamera::Move(float speed){
    direction=m_view-m_position;
    direction=direction.normalize();
    m_position+=direction*speed;
    m_view+=direction*speed;
}

void CCamera::Strafe(float speed)
{
	m_position.x += m_strafe.x * speed;
	m_position.z += m_strafe.z * speed;

	// Add the strafe vector to our view
	m_view.x += m_strafe.x * speed;
	m_view.z += m_strafe.z * speed;
}

void CCamera::Update(){
    m_strafe = m_view - m_position;
	//m_strafe = m_up.cross(m_strafe);
	m_strafe = m_strafe.cross(m_up);
	// Normalize the strafe vector
	m_strafe = m_strafe.normalize();
}

void CCamera::RotateView(float angle, float x, float y, float z)
{
    Vector3f vNewView;

    // Получим наш вектор взгляда (направление, куда мы смотрим)
    direction=m_view-m_position;
    direction=direction.normalize();

    // Теперь, имея вектор взгляда, хранящийся в CVector3 "vView", мы можем вращать его.
    // Эта ф-я будет вызыватся, когда нужно повернутся налево-направо.
    // Итак, нам нужно вращаться вокруг нашей позиции. Представьте это примерно так:
    // скажем, мы стоим на месте, смотря вперед. Мы смотрим в какую-то точку, верно?
    // Теперь, если мы повернем голову налево или направо, направление взгляда изменится.
    // Соответственно переместится точка, на которую мы смотрим (вектор взгляда).
    // Вот почему мы изменяем m_vView - потому что это и есть вектор
    // взгляда. Мы будем вращать m_vView вокруг m_vPosition
    // по кругу, чтобы реализовать всё это.
    // Чтобы вращать что-то, используем синус и косинус. Для использования sin() & cos() мы
    // ранее подключили math.h.
    //
    // Чтобы реализовать вращение камеры, мы будем использовать axis-angle вращение.
    // Я должен предупредить, что формулы для рассчета вращения не очень просты, но
    // занимают не много кода. Axis-angle вращение позволяет нам вращать точку в пространстве
    // вокруг нужной оси. Это значит, что мы можем взять нашу точку взгляда (m_vView) и
    // вращать вокруг нашей позиции.
    // Чтобы лучше понять следующие рассчеты, советую вам посмотреть детальное
    // описание: http://astronomy.swin.edu.au/~pbourke/geometry/rotate/

    // Рассчитаем 1 раз синус и косинус переданного угла
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    // Найдем новую позицию X для вращаемой точки
    vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)   * direction.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)   * direction.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)   * direction.z;

    // Найдем позицию Y
    vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)   * direction.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y)   * direction.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)   * direction.z;

    // И позицию Z
    vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)   * direction.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)   * direction.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z)   * direction.z;

    // Теперь просто добавим новый вектор вращения к нашей позиции, чтобы
    // установить новый взгляд камеры.
    m_view=m_position+vNewView;
    this->Update();
}

void CCamera::MouseRotate(int dx, int dy){
    float angleY = -dx/500.0f;    // Направление взгляда вверх/вниз
    float angleZ = -dy/500.0f;    // Значение, необходимое для вращения влево-вправо (по оси Y)
    static float currentRotX = 0.0f;
    static float lastRotX = 0.0f;
    lastRotX = currentRotX;     // Сохраняем последний угол вращения

// Если текущее вращение больше 1 градуса, обрежем его, чтобы не вращать слишком быстро
    if(currentRotX > 1.0f)
    {
        currentRotX = 1.0f;

        // врощаем на оставшийся угол
        if(lastRotX != 1.0f)
        {
            // Чтобы найти ось, вокруг которой вращаться вверх и вниз, нужно
            // найти вектор, перпендикулярный вектору взгляда камеры и
            // вертикальному вектору.
            // Это и будет наша ось. И прежде чем использовать эту ось,
            // неплохо бы нормализовать её.
            Vector3f vAxis = (m_view - m_position);
            vAxis=vAxis.cross(m_up);
            vAxis = vAxis.normalize();

            // Вращаем камеру вокруг нашей оси на заданный угол
            RotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
        }
    }

    // Если угол меньше -1.0f, убедимся, что вращение не продолжится
    else if(currentRotX < -1.0f)
    {
        currentRotX = -1.0f;
        if(lastRotX != -1.0f)
        {
            // Опять же вычисляем ось
            Vector3f vAxis = (m_view - m_position);
            vAxis=vAxis.cross(m_up);
            vAxis = vAxis.normalize();

            // Вращаем
            RotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
        }
    }
    // Если укладываемся в пределы 1.0f -1.0f - просто вращаем
    else
    {
        Vector3f vAxis = m_view - m_position;
        vAxis=vAxis.cross(m_up);
        vAxis = vAxis.normalize();
        RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
    }

    // Всегда вращаем камеру вокруг Y-оси
    RotateView(angleY, 0, 1, 0);
    this->Update();
}

void CCamera::RotateAroundPoint(Vector3f vCenter, float angle, float x, float y, float z)
{
    Vector3f vNewPosition;

    // Чтобы вращать нашу позицию вокруг точки, всё, что нам надо - найти вектор
    // от нашей позиции к центральной точке вращения. Один раз получив вектор,
    // мы вращаемся вокруг точки с заданной скоростью/углом.
    // Новый вектор vCenter - точка, вокруг которой мы вращаемся.

    // Получим центр, вокруг которого нужно вращатся
    Vector3f vPos = m_position - vCenter;

    // Вычислим синус и косинус угла
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    // Найдем значение X точки вращения
    vNewPosition.x  = (cosTheta + (1 - cosTheta) * x * x)       * vPos.x;
    vNewPosition.x += ((1 - cosTheta) * x * y - z * sinTheta)   * vPos.y;
    vNewPosition.x += ((1 - cosTheta) * x * z + y * sinTheta)   * vPos.z;

    // И значение Y
    vNewPosition.y  = ((1 - cosTheta) * x * y + z * sinTheta)   * vPos.x;
    vNewPosition.y += (cosTheta + (1 - cosTheta) * y * y)       * vPos.y;
    vNewPosition.y += ((1 - cosTheta) * y * z - x * sinTheta)   * vPos.z;

    // И Z...
    vNewPosition.z  = ((1 - cosTheta) * x * z - y * sinTheta)   * vPos.x;
    vNewPosition.z += ((1 - cosTheta) * y * z + x * sinTheta)   * vPos.y;
    vNewPosition.z += (cosTheta + (1 - cosTheta) * z * z)       * vPos.z;

    // Теперь просто прибавим новый вектор к нашей позиции,
    // чтобы установить новую позицию камеры.
    m_position = vCenter + vNewPosition;
    this->Update();
}


