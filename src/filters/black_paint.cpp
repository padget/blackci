#include <iostream>
#include <regex>
#include <algorithm>

#include <fmt/color.h>
#include <yaml-cpp/yaml.h>

struct format
{
  std::regex reg;
  fmt::text_style style;
};

struct filter
{
  std::string name;
  std::vector<format> formats;
};

struct configuration
{
  std::string path;
  std::vector<filter> filters;
};

fmt::text_style
to_style(
    const std::vector<std::string> &styles);

std::string
get_config_path(int argc, char **argv);

std::string
get_filter_name(int argc, char **argv);

configuration
load_configuration(const std::string &name);

const filter &
get_filter(
    const std::string &name,
    const std::vector<filter> &filters);

void apply_filter(
    const std::string &str,
    const filter &f);

int main(int argc, char **argv)
{
  auto path = get_config_path(argc, argv);
  auto fname = get_filter_name(argc, argv);
  auto config = load_configuration(path);
  auto &filter = get_filter(fname, config.filters);

  std::string line;

  while (std::getline(std::cin, line))
    apply_filter(line, filter);

  return EXIT_SUCCESS;
}

fmt::text_style
to_style(
    const std::vector<std::string> &styles)
{
  fmt::text_style style;

  for (auto &s : styles)
  {
    if (s == "bold")
      style |= fmt::emphasis::bold;
    if (s == "italic")
      style |= fmt::emphasis::italic;
    if (s == "strikethrough")
      style |= fmt::emphasis::strikethrough;
    if (s == "underline")
      style |= fmt::emphasis::underline;
    if (s == "alice_blue")
      style |= fmt::fg(fmt::color::alice_blue);
    if (s == "antique_white")
      style |= fmt::fg(fmt::color::antique_white);
    if (s == "aqua")
      style |= fmt::fg(fmt::color::aqua);
    if (s == "aquamarine")
      style |= fmt::fg(fmt::color::aquamarine);
    if (s == "azure")
      style |= fmt::fg(fmt::color::azure);
    if (s == "beige")
      style |= fmt::fg(fmt::color::beige);
    if (s == "bisque")
      style |= fmt::fg(fmt::color::bisque);
    if (s == "black")
      style |= fmt::fg(fmt::color::black);
    if (s == "blanched_almond")
      style |= fmt::fg(fmt::color::blanched_almond);
    if (s == "blue")
      style |= fmt::fg(fmt::color::blue);
    if (s == "blue_violet")
      style |= fmt::fg(fmt::color::blue_violet);
    if (s == "brown")
      style |= fmt::fg(fmt::color::brown);
    if (s == "burly_wood")
      style |= fmt::fg(fmt::color::burly_wood);
    if (s == "cadet_blue")
      style |= fmt::fg(fmt::color::cadet_blue);
    if (s == "chartreuse")
      style |= fmt::fg(fmt::color::chartreuse);
    if (s == "chocolate")
      style |= fmt::fg(fmt::color::chocolate);
    if (s == "coral")
      style |= fmt::fg(fmt::color::coral);
    if (s == "cornflower_blue")
      style |= fmt::fg(fmt::color::cornflower_blue);
    if (s == "cornsilk")
      style |= fmt::fg(fmt::color::cornsilk);
    if (s == "crimson")
      style |= fmt::fg(fmt::color::crimson);
    if (s == "cyan")
      style |= fmt::fg(fmt::color::cyan);
    if (s == "dark_blue")
      style |= fmt::fg(fmt::color::dark_blue);
    if (s == "dark_cyan")
      style |= fmt::fg(fmt::color::dark_cyan);
    if (s == "dark_golden_rod")
      style |= fmt::fg(fmt::color::dark_golden_rod);
    if (s == "dark_gray")
      style |= fmt::fg(fmt::color::dark_gray);
    if (s == "dark_green")
      style |= fmt::fg(fmt::color::dark_green);
    if (s == "dark_khaki")
      style |= fmt::fg(fmt::color::dark_khaki);
    if (s == "dark_magenta")
      style |= fmt::fg(fmt::color::dark_magenta);
    if (s == "dark_olive_green")
      style |= fmt::fg(fmt::color::dark_olive_green);
    if (s == "dark_orange")
      style |= fmt::fg(fmt::color::dark_orange);
    if (s == "dark_orchid")
      style |= fmt::fg(fmt::color::red);
    if (s == "dark_red")
      style |= fmt::fg(fmt::color::dark_red);
    if (s == "dark_salmon")
      style |= fmt::fg(fmt::color::dark_salmon);
    if (s == "dark_sea_green")
      style |= fmt::fg(fmt::color::dark_sea_green);
    if (s == "dark_slate_blue")
      style |= fmt::fg(fmt::color::dark_slate_blue);
    if (s == "dark_slate_gray")
      style |= fmt::fg(fmt::color::dark_slate_gray);
    if (s == "dark_turquoise")
      style |= fmt::fg(fmt::color::dark_turquoise);
    if (s == "dark_violet")
      style |= fmt::fg(fmt::color::dark_violet);
    if (s == "deep_pink")
      style |= fmt::fg(fmt::color::deep_pink);
    if (s == "deep_sky_blue")
      style |= fmt::fg(fmt::color::deep_sky_blue);
    if (s == "dim_gray")
      style |= fmt::fg(fmt::color::dim_gray);
    if (s == "dodger_blue")
      style |= fmt::fg(fmt::color::dodger_blue);
    if (s == "fire_brick")
      style |= fmt::fg(fmt::color::fire_brick);
    if (s == "floral_white")
      style |= fmt::fg(fmt::color::floral_white);
    if (s == "forest_green")
      style |= fmt::fg(fmt::color::forest_green);
    if (s == "fuchsia")
      style |= fmt::fg(fmt::color::fuchsia);
    if (s == "gainsboro")
      style |= fmt::fg(fmt::color::gainsboro);
    if (s == "ghost_white")
      style |= fmt::fg(fmt::color::ghost_white);
    if (s == "gold")
      style |= fmt::fg(fmt::color::gold);
    if (s == "golden_rod")
      style |= fmt::fg(fmt::color::golden_rod);
    if (s == "gray")
      style |= fmt::fg(fmt::color::gray);
    if (s == "green")
      style |= fmt::fg(fmt::color::green);
    if (s == "green_yellow")
      style |= fmt::fg(fmt::color::green_yellow);
    if (s == "honey_dew")
      style |= fmt::fg(fmt::color::honey_dew);
    if (s == "hot_pink")
      style |= fmt::fg(fmt::color::hot_pink);
    if (s == "red")
      style |= fmt::fg(fmt::color::red);
    if (s == "red")
      style |= fmt::fg(fmt::color::red);
    if (s == "indigo")
      style |= fmt::fg(fmt::color::indigo);
    if (s == "ivory")
      style |= fmt::fg(fmt::color::ivory);
    if (s == "khaki")
      style |= fmt::fg(fmt::color::khaki);
    if (s == "lavender")
      style |= fmt::fg(fmt::color::lavender);
    if (s == "lavender_blush")
      style |= fmt::fg(fmt::color::lavender_blush);
    if (s == "lawn_green")
      style |= fmt::fg(fmt::color::lawn_green);
    if (s == "lemon_chiffon")
      style |= fmt::fg(fmt::color::lemon_chiffon);
    if (s == "light_blue")
      style |= fmt::fg(fmt::color::light_blue);
    if (s == "light_coral")
      style |= fmt::fg(fmt::color::light_coral);
    if (s == "light_cyan")
      style |= fmt::fg(fmt::color::light_cyan);
    if (s == "light_golden_rod_yellow")
      style |= fmt::fg(fmt::color::light_golden_rod_yellow);
    if (s == "light_gray")
      style |= fmt::fg(fmt::color::light_gray);
    if (s == "light_green")
      style |= fmt::fg(fmt::color::light_green);
    if (s == "light_pink")
      style |= fmt::fg(fmt::color::light_pink);
    if (s == "light_salmon")
      style |= fmt::fg(fmt::color::light_salmon);
    if (s == "light_sea_green")
      style |= fmt::fg(fmt::color::light_sea_green);
    if (s == "light_sky_blue")
      style |= fmt::fg(fmt::color::light_sky_blue);
    if (s == "light_slate_gray")
      style |= fmt::fg(fmt::color::light_slate_gray);
    if (s == "light_steel_blue")
      style |= fmt::fg(fmt::color::light_steel_blue);
    if (s == "light_yellow")
      style |= fmt::fg(fmt::color::light_yellow);
    if (s == "lime")
      style |= fmt::fg(fmt::color::lime);
    if (s == "lime_green")
      style |= fmt::fg(fmt::color::lime_green);
    if (s == "linen")
      style |= fmt::fg(fmt::color::linen);
    if (s == "magenta")
      style |= fmt::fg(fmt::color::magenta);
    if (s == "maroon")
      style |= fmt::fg(fmt::color::maroon);
    if (s == "medium_aquamarine")
      style |= fmt::fg(fmt::color::medium_aquamarine);
    if (s == "medium_blue")
      style |= fmt::fg(fmt::color::medium_blue);
    if (s == "medium_orchid")
      style |= fmt::fg(fmt::color::medium_orchid);
    if (s == "medium_purple")
      style |= fmt::fg(fmt::color::medium_purple);
    if (s == "medium_sea_green")
      style |= fmt::fg(fmt::color::medium_sea_green);
    if (s == "medium_slate_blue")
      style |= fmt::fg(fmt::color::medium_slate_blue);
    if (s == "medium_spring_green")
      style |= fmt::fg(fmt::color::medium_spring_green);
    if (s == "medium_turquoise")
      style |= fmt::fg(fmt::color::medium_turquoise);
    if (s == "medium_violet_red")
      style |= fmt::fg(fmt::color::medium_violet_red);
    if (s == "midnight_blue")
      style |= fmt::fg(fmt::color::midnight_blue);
    if (s == "mint_cream")
      style |= fmt::fg(fmt::color::mint_cream);
    if (s == "misty_rose")
      style |= fmt::fg(fmt::color::misty_rose);
    if (s == "moccasin")
      style |= fmt::fg(fmt::color::moccasin);
    if (s == "navajo_white")
      style |= fmt::fg(fmt::color::navajo_white);
    if (s == "navy")
      style |= fmt::fg(fmt::color::navy);
    if (s == "old_lace")
      style |= fmt::fg(fmt::color::old_lace);
    if (s == "olive")
      style |= fmt::fg(fmt::color::olive);
    if (s == "olive_drab")
      style |= fmt::fg(fmt::color::olive_drab);
    if (s == "orange")
      style |= fmt::fg(fmt::color::orange);
    if (s == "orange_red")
      style |= fmt::fg(fmt::color::orange_red);
    if (s == "orchid")
      style |= fmt::fg(fmt::color::orchid);
    if (s == "pale_golden_rod")
      style |= fmt::fg(fmt::color::pale_golden_rod);
    if (s == "pale_green")
      style |= fmt::fg(fmt::color::pale_green);
    if (s == "pale_turquoise")
      style |= fmt::fg(fmt::color::pale_turquoise);
    if (s == "pale_violet_red")
      style |= fmt::fg(fmt::color::pale_violet_red);
    if (s == "papaya_whip")
      style |= fmt::fg(fmt::color::papaya_whip);
    if (s == "peach_puff")
      style |= fmt::fg(fmt::color::peach_puff);
    if (s == "peru")
      style |= fmt::fg(fmt::color::peru);
    if (s == "pink")
      style |= fmt::fg(fmt::color::pink);
    if (s == "plum")
      style |= fmt::fg(fmt::color::plum);
    if (s == "powder_blue")
      style |= fmt::fg(fmt::color::powder_blue);
    if (s == "purple")
      style |= fmt::fg(fmt::color::purple);
    if (s == "rebecca_purple")
      style |= fmt::fg(fmt::color::rebecca_purple);
    if (s == "red")
      style |= fmt::fg(fmt::color::red);
    if (s == "rosy_brown")
      style |= fmt::fg(fmt::color::rosy_brown);
    if (s == "royal_blue")
      style |= fmt::fg(fmt::color::royal_blue);
    if (s == "saddle_brown")
      style |= fmt::fg(fmt::color::saddle_brown);
    if (s == "salmon")
      style |= fmt::fg(fmt::color::red);
    if (s == "sandy_brown")
      style |= fmt::fg(fmt::color::sandy_brown);
    if (s == "sea_green")
      style |= fmt::fg(fmt::color::sea_green);
    if (s == "sea_shell")
      style |= fmt::fg(fmt::color::sea_shell);
    if (s == "sienna")
      style |= fmt::fg(fmt::color::sienna);
    if (s == "silver")
      style |= fmt::fg(fmt::color::silver);
    if (s == "sky_blue")
      style |= fmt::fg(fmt::color::sky_blue);
    if (s == "slate_blue")
      style |= fmt::fg(fmt::color::slate_blue);
    if (s == "slate_gray")
      style |= fmt::fg(fmt::color::slate_gray);
    if (s == "snow")
      style |= fmt::fg(fmt::color::snow);
    if (s == "spring_green")
      style |= fmt::fg(fmt::color::spring_green);
    if (s == "steel_blue")
      style |= fmt::fg(fmt::color::steel_blue);
    if (s == "tan")
      style |= fmt::fg(fmt::color::tan);
    if (s == "teal")
      style |= fmt::fg(fmt::color::teal);
    if (s == "thistle")
      style |= fmt::fg(fmt::color::thistle);
    if (s == "tomato")
      style |= fmt::fg(fmt::color::tomato);
    if (s == "turquoise")
      style |= fmt::fg(fmt::color::turquoise);
    if (s == "violet")
      style |= fmt::fg(fmt::color::violet);
    if (s == "wheat")
      style |= fmt::fg(fmt::color::wheat);
    if (s == "white ")
      style |= fmt::fg(fmt::color::white);
    if (s == "white_smoke")
      style |= fmt::fg(fmt::color::white_smoke);
    if (s == "yellow")
      style |= fmt::fg(fmt::color::yellow);
    if (s == "yellow_green")
      style |= fmt::fg(fmt::color::yellow_green);
  }

  return style;
}

std::string
get_config_path(int argc, char **argv)
{
  std::string path;

  return path;
}

std::string
get_filter_name(int argc, char **argv)
{
  std::string name;

  return name;
}

configuration
load_configuration(const std::string &name)
{
  configuration config;

  return config;
}

const filter &
get_filter(
    const std::string &name,
    const std::vector<filter> &filters)
{
  auto ifilter = std::find_if(
      filters.begin(), filters.end(),
      [&name](const filter &flt) {
        return flt.name == name;
      });

  if (ifilter != filters.end())
    return *ifilter;
  else
    throw std::exception();
}

void apply_filter(
    const std::string &str,
    const filter &f)
{
}
