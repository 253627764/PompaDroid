#include "OptionLayer.h"

using namespace cocos2d;

OptionLayer::OptionLayer()
{}

OptionLayer::~OptionLayer()
{}

bool OptionLayer::init()
{
    bool ret = false;

    do {
        CC_BREAK_IF(!Layer::init());

        _joystick = Sprite::create("joystick.png");
        _joystick_bg = Sprite::create("joystick_bg.png");
        this->addChild(_joystick_bg);
        this->addChild(_joystick);

        _inactivityJoystick();

        setTouchEnabled(true);

        ret = true;
    } while(0);

    return ret;
}

void OptionLayer::ccTouchesBegan(Set *ts, Event *e)
{
    Size winSize = Director::getInstance()->getWinSize();
    SetIterator iter = ts->begin();

    while (iter != ts->end()) {
        Touch *t = (Touch*)(*iter);
        Point p = t->getLocation();
        // left�������ز��������С����Ļ��ȵ�һ�룬˵�����ط���������
        if (p.x <= winSize.width / 2) {
            _activityJoystick(p);
        } else {
            // right������������������������������ź�
            _delegator->onAttack();
        }

        iter++;
    }
}

void OptionLayer::ccTouchesMoved(Set *ts, Event *e)
{
    Size winSize = Director::getInstance()->getWinSize();
    SetIterator iter = ts->begin();
    Touch *t = (Touch*)(*iter);
    Point start = t->getStartLocation();

    // ����ô��ص���������������ģ�����������������
    if (start.x > winSize.width / 2) {
        return;
    }

    Point p = t->getLocation();
    // ��ȡλ�Ƶ�����ʼ���ƫ������ֱ�߾��룩
    float distance = start.getDistance(p);
    // ��ȡ��ʼ�㵽λ�Ƶ����������λΪ1�����꣩
    Point direction = (p - start).normalize();

    _updateJoystick(direction, distance);
    _delegator->onWalk(direction, distance);
}

void OptionLayer::ccTouchesEnded(Set *ts, Event *e)
{
    if (_joystick_bg->isVisible()) {
        _inactivityJoystick();
        _delegator->onStop();
    }
}

void OptionLayer::_activityJoystick(Point position)
{
    _joystick->setPosition(position);
    _joystick_bg->setPosition(position);
    _joystick->setVisible(true);
    _joystick_bg->setVisible(true);
}

void OptionLayer::_inactivityJoystick()
{
    _joystick->setPosition(_joystick_bg->getPosition());
    _joystick->setVisible(false);
    _joystick_bg->setVisible(false);
}

void OptionLayer::_updateJoystick(Point direction, float distance)
{
    // �ԡ�ҡ�˻�������Բ��Ϊ�������ο�����ҡ�ˡ�����Ӧ�ı���
    Point start = _joystick_bg->getPosition();

    if (distance < 32) {
        // ����ƶ�δ������ҡ�˻���������ҡ�ˡ��ڡ���������ƫ��
        _joystick->setPosition(start + (direction * distance));
    } else if (distance > 96) {
        // ����ƶ�������ҡ�˻���������ҡ�ˡ�Բ��ʼ���ڡ���������Ե��ƫ��
        _joystick->setPosition(start + (direction * 64));
    } else {
        // ����ƶ�������ҡ�˻���������ҡ�ˡ���Եʼ���ڡ���������Ե��ƫ��
        _joystick->setPosition(start + (direction * 32));
    }
}
