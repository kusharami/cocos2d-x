#pragma once

#include "platform/CCStdC.h"
#include "platform/CCApplication.h"

#include <QOpenGLWindow>
#include <QColor>

#include <functional>

namespace cocos2d
{
class Scene;
class Node;
class GLViewImpl;
} // namespace cocos2d

class QtCocosWindow
	: public QOpenGLWindow
	, public cocos2d::Application
{
	Q_OBJECT

	Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)

public:
	QtCocosWindow();
	virtual ~QtCocosWindow() override;

	static QWidget *createWidget(QWidget *parent);
	inline QWidget *masterWidget() const;

	QWidget *makeContainerWidget(QWidget *parent);

	bool isAnimating() const;
	void animate(bool yes);

	inline int scale() const;
	void setScale(int value);

	inline bool isFixedWheelScale() const;
	inline void setFixedWheelScale(bool fixed);

	inline cocos2d::Node *mainNode() const;
	cocos2d::Scene *scene() const;
	inline bool hasFocus() const;

	static QtCocosWindow *instance();

	inline bool isInitialized() const;

	QColor backgroundColor() const;
	void setBackgroundColor(const QColor &color);

	inline bool isEnabled() const;
	inline void setEnabled(bool enabled);

public:
	virtual bool applicationDidFinishLaunching() override;
	virtual void applicationDidEnterBackground() override;
	virtual void applicationWillEnterForeground() override;
	virtual void setAnimationInterval(float) override;

protected:
	virtual bool event(QEvent *e) override;
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
	virtual void focusInEvent(QFocusEvent *event) override;
	virtual void focusOutEvent(QFocusEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void wheelEvent(QWheelEvent *event) override;
	virtual void showEvent(QShowEvent *) override;
	virtual void exposeEvent(QExposeEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;

signals:
	void Initialized();
	void VisibleFrameAdjusted();
	void ScaleChanged();
	void KeyDown(QKeyEvent *event);
	void KeyUp(QKeyEvent *event);
	void MousePressed(QMouseEvent *event);
	void MouseMoved(QMouseEvent *event);
	void MouseReleased(QMouseEvent *event);
	void FocusIn();
	void FocusOut();
	void MouseEnter();
	void MouseLeave();

private:
	class InternalMainNode;
	friend class InternalMainNode;

	void updateAnimationState(bool force);

	float getScaleForMainNode() const;
	void updateMainNodeScale();
	static bool isContextMenuEvent(QMouseEvent *event);
	void fixScale();

	enum
	{
		ANIMATING = 1 << 0,
		EXPOSED = 1 << 1,
		BEGIN_NO_TOUCH = 1 << 2
	};

	cocos2d::GLViewImpl *mEGLView;
	cocos2d::Node *mMainNode;
	cocos2d::Scene *mScene;
	int mScale;
	quint32 mMouseButtons;
	quint32 mState;
	QColor mBgColor;
	QWidget *mMasterWidget;
	bool mCanShowContextMenu = true;
	bool mHasFocus;
	bool mEnabled;
	bool mRunning;
	bool mInitialized;
	bool mFixedWheelScale;
};

QWidget *QtCocosWindow::masterWidget() const
{
	return mMasterWidget;
}

int QtCocosWindow::scale() const
{
	return mScale;
}

bool QtCocosWindow::isFixedWheelScale() const
{
	return mFixedWheelScale;
}

void QtCocosWindow::setFixedWheelScale(bool fixed)
{
	mFixedWheelScale = fixed;
}

cocos2d::Node *QtCocosWindow::mainNode() const
{
	return mMainNode;
}

bool QtCocosWindow::hasFocus() const
{
	return mHasFocus;
}

bool QtCocosWindow::isInitialized() const
{
	return mInitialized;
}

bool QtCocosWindow::isEnabled() const
{
	return mEnabled;
}

void QtCocosWindow::setEnabled(bool enabled)
{
	mEnabled = enabled;
}
