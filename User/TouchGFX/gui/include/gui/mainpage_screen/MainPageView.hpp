#ifndef MAINPAGEVIEW_HPP
#define MAINPAGEVIEW_HPP

#include <gui_generated/mainpage_screen/MainPageViewBase.hpp>
#include <gui/mainpage_screen/MainPagePresenter.hpp>

class MainPageView : public MainPageViewBase
{
public:
    MainPageView();
    virtual ~MainPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MAINPAGEVIEW_HPP
