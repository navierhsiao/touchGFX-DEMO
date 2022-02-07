/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINPAGEVIEWBASE_HPP
#define MAINPAGEVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/mainpage_screen/MainPagePresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/SwipeContainer.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/SlideMenu.hpp>

class MainPageViewBase : public touchgfx::View<MainPagePresenter>
{
public:
    MainPageViewBase();
    virtual ~MainPageViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box box1;
    touchgfx::Container container1;
    touchgfx::Box box2;
    touchgfx::SwipeContainer swipeContainer1;
    touchgfx::Container page2;
    touchgfx::ButtonWithLabel buttonWithLabel2;
    touchgfx::TextArea textArea1_1_1;
    touchgfx::TextArea textArea1_2;
    touchgfx::Container page1;
    touchgfx::ButtonWithLabel buttonWithLabel1;
    touchgfx::TextArea textArea1_1;
    touchgfx::TextArea textArea1;
    touchgfx::SlideMenu slideMenu1;
    touchgfx::ButtonWithLabel buttonWithLabel3;
    touchgfx::ButtonWithLabel buttonWithLabel3_1;
    touchgfx::ButtonWithLabel buttonWithLabel3_2;
    touchgfx::ButtonWithLabel buttonWithLabel3_3;

private:

};

#endif // MAINPAGEVIEWBASE_HPP
