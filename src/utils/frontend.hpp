#ifndef __black_frontend_hpp__
#define __black_frontend_hpp__

#include <fmt/core.h>
#include <fmt/color.h>

#include <string>

namespace black::front
{
  namespace style
  {
    std::string truncate(
        const std::string &str,
        size_t width)
    {
      if (str.length() > width - 3)
        return str.substr(0, width - 3) + "...";

      return str;
    }

    auto bold()
    {
      return fmt::emphasis::bold;
    }

    auto fg_deep_sky_blue()
    {
      return fmt::fg(fmt::color::deep_sky_blue);
    }

    auto fg_black()
    {
      return fmt::fg(fmt::color::black);
    }

    auto fg_gray()
    {
      return fmt::fg(fmt::color::gray);
    }
  } // namespace style

  auto project_title(const std::string &title)
  {
    auto style = style::bold() |
                 style::fg_deep_sky_blue();
    auto trtitle = style::truncate(title, 45);
    auto ss = std::stringstream();
    ss << "[" << trtitle << "] ";
    auto sqtitle = ss.str();
    return fmt::format(style, "PROJECT {:*<50}\n", sqtitle);
  }

  auto project_package(const std::string &title,
                       const std::string &package)
  {
    auto style = style::bold();
    auto trtitle = style::truncate(title + " : " + package, 45);
    auto ss = std::stringstream();
    ss << "[" << trtitle << "] ";
    auto sqtitle = ss.str();
    return fmt::format(style, "PACKAGE {:*<50}\n", sqtitle);
  }

  auto comment(const std::string& com)
  {
    auto style = style::bold() | style::fg_gray();
    return fmt::format(style, "# {}\n", com);
  }
} // namespace black::front

#endif
