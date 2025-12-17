#include <phoenix/root.h>
#include <phoenix/drivers/gfx/gl/glrend.h>
#include <phoenix/drivers/ui/uicontroller.h>

#include <cppual/ui/window.h>
#include <cppual/ui/queue.h>
#include <cppual/compute/task.h>
#include <cppual/input/pointer.h>
#include <cppual/containers.h>
#include <cppual/memory/system.h>
#include <cppual/memory/allocator.h>
#include <cppual/multimedia/audio/al/music.h>
#include <cppual/interface.h>
#include <cppual/system/clock.h>
#include <cppual/string_helper.h>

#include <iostream>

// =========================================================

class main_window : public cppual::ui::window
{
public:
    using cppual::ui::window::window;

private:
    void paint_event (cppual::rect const&)
    {
        using cppual::point2i;
        using cppual::gfx::color;
        typedef cppual::gfx::drawable_interface::line_style line_style;

        cppual::gfx::painter paint (platform_surface ());

        paint.create_line ({ 20, 20, 150, 0 }, { 0, 0, 0 }, 1, line_style::dash_dot_dot);
        paint.create_ellipse ({ 200, 50, 150, 130 }, { 0, 0, 0 }, { 0, 0, 0 });
        paint.create_path ({ point2i (20, 200), point2i (100, 250), point2i (150, 120) }, { 0, 0, 0 },
                             1, line_style::dash_dot_dot);
        paint.create_rectangle ({ 20, 270, 150, 100 }, { 0, 0, 150 }, { 0, 0, 0 });
        paint.create_polygon ({ point2i (200, 300), point2i (300, 300), point2i (250, 450) },
                             { 0, 100, 0 }, { 100, 0, 0 });
    }
};

// =========================================================

class test_derived_interface : public cppual::unbound_interface
{
public:
    template <typename... Ts>
    test_derived_interface (Ts... pairs)
    : unbound_interface (std::pair { "test_function", &test_derived_interface::test_function },
                         pairs...)
    {
        call ("test_function" );
        call ("test2_function");
    }

    void test_function ()
    {
        std::cout << "unbound interface fn count: " << interface_fn_count () << std::endl;
    }

    virtual void virtual_test_function ()
    {
        std::cout << "[virtual_test_function] unbound interface fn count: " << interface_fn_count ()
                  << std::endl;
    }

    void call_virtual_fn ()
    {
        call ("virtual_test_function");
    }
};

// =========================================================

class test_2_interface : public test_derived_interface
{
public:
    test_2_interface ()
    : test_derived_interface (std::pair { "test2_function", &test_2_interface::test2_function },
                              std::pair { "virtual_test_function", &test_2_interface::virtual_test_function })
    {
    }

    void test2_function ()
    {
        std::cout << "[test2_function] unbound interface fn count: " << interface_fn_count ()
                  << std::endl;
    }

    void virtual_test_function ()
    {
        std::cout << "[virtual_test_function -> overriden] unbound interface fn count: "
                  << interface_fn_count () << std::endl;
    }
};

// =========================================================

template <typename Hasher = std::hash<cppual::cchar*>>
void benchmark_unordered_map ()
{
    typedef cppual::unordered_map<cppual::cchar*, cppual::function<void()>, Hasher> map_type;

    std::size_t count = 100000000;
    cppual::clock::timer timer;
    map_type map;

    map.reserve (count);

    for (auto i = 0U; i < count; ++i)
    {

    }

    std::cout << "performance :: unordered_map :: "
              << typeid (Hasher).name () << ": "
              << timer.elapsed<std::chrono::milliseconds> ().count () << " ms" << std::endl;
}

// =========================================================

int main ()
{
    cppual::memory::set_default_resource (*cppual::memory::system_resource ());

    test_2_interface test;

    test.call_virtual_fn ();

    benchmark_unordered_map<cppual::unbound_interface::key_hash> ();
    benchmark_unordered_map ();

    cppual::audio::al::sound_buffer buffer;
    cppual::audio::al::sound_player player (buffer);

//    cppual::compute::host_task<int> task1  ([]
//    {
//        std::cout << "task1...";
//        return 1;
//    });

//    cppual::compute::host_task<int> task2 ([]
//    {
//        std::cout << "task2" << std::endl;
//        return 2;
//    });

//    std::cout << task1 () << ' ' << task2 () << std::endl;

    buffer.open_read_only ("/media/multimedia/Music/05 - The Crystal Method - Busy Child.mp3");
    player.play ();

    //main_window wnd (nullptr, cppual::rect (0, 0, 640, 480), "Test");
    cppual::ui::window wnd (nullptr, cppual::rect (0, 0, 640, 480), "Test");
    //cppual::ui::event_queue queue;

    Phoenix::Ui::Controller ui (wnd);
    Phoenix::Visual::GLRasterRenderer renderer (wnd);

    Phoenix::Kernel engine;

    if (!engine.addDriver (ui)       ||
        !engine.addDriver (renderer) ||
        !engine.load ())
    {
       return 1;
    }

    //wnd.show_minimized ();
    wnd.show ();

    cppual::connect (cppual::ui::event_queue::events ().mousePress,
                     [&](cppual::ui::event_queue::handle_type elem, cppual::input::sys_event::mbutton_data data)
    {
        if (elem != wnd.platform_handle ()) return;

        if (data.button == cppual::input::mouse::right)
        {
            if (wnd.is_fullscreen ())
            {
                wnd.set_fullscreen (false);
                std::cout << "Exit Fullscreen" << std::endl;
            }
            else
            {
                wnd.set_fullscreen (true);
                std::cout << "Enter Fullscreen" << std::endl;
            }
        }
    });

    /*cppual::connect (cppual::ui::event_queue::events ().winVisible,
                     [&](cppual::ui::event_queue::handle_type elem, bool bVis)
    {
        if (elem == wnd.platform_handle () && !bVis) wnd.restore ();
    });*/

    cppual::connect (cppual::ui::event_queue::events ().winClose,
                     [&](cppual::ui::event_queue::handle_type elem)
    {
        if (elem == wnd.platform_handle ()) engine.quit ();
        //if (elem == wnd.platform_handle ()) queue.quit ();
    });

    return engine.exec ();
    //return queue.poll ();
}
