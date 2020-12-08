#ifndef __filters_fmt_to_style_hpp__
#define __filters_fmt_to_style_hpp__

#include <fmt/color.h>

fmt::text_style
to_style(
    const std::vector<std::string> &styles)
{
  fmt::text_style style;

  for (auto &s : styles)
  {
    if (s == "bold")
      style |= fmt::emphasis::bold;
    else if (s == "italic")
      style |= fmt::emphasis::italic;
    else if (s == "strikethrough")
      style |= fmt::emphasis::strikethrough;
    else if (s == "underline")
      style |= fmt::emphasis::underline;
    else if (s == "alice_blue")
      style |= fmt::fg(fmt::color::alice_blue);
    else if (s == "antique_white")
      style |= fmt::fg(fmt::color::antique_white);
    else if (s == "aqua")
      style |= fmt::fg(fmt::color::aqua);
    else if (s == "aquamarine")
      style |= fmt::fg(fmt::color::aquamarine);
    else if (s == "azure")
      style |= fmt::fg(fmt::color::azure);
    else if (s == "beige")
      style |= fmt::fg(fmt::color::beige);
    else if (s == "bisque")
      style |= fmt::fg(fmt::color::bisque);
    else if (s == "black")
      style |= fmt::fg(fmt::color::black);
    else if (s == "blanched_almond")
      style |= fmt::fg(fmt::color::blanched_almond);
    else if (s == "blue")
      style |= fmt::fg(fmt::color::blue);
    else if (s == "blue_violet")
      style |= fmt::fg(fmt::color::blue_violet);
    else if (s == "brown")
      style |= fmt::fg(fmt::color::brown);
    else if (s == "burly_wood")
      style |= fmt::fg(fmt::color::burly_wood);
    else if (s == "cadet_blue")
      style |= fmt::fg(fmt::color::cadet_blue);
    else if (s == "chartreuse")
      style |= fmt::fg(fmt::color::chartreuse);
    else if (s == "chocolate")
      style |= fmt::fg(fmt::color::chocolate);
    else if (s == "coral")
      style |= fmt::fg(fmt::color::coral);
    else if (s == "cornflower_blue")
      style |= fmt::fg(fmt::color::cornflower_blue);
    else if (s == "cornsilk")
      style |= fmt::fg(fmt::color::cornsilk);
    else if (s == "crimson")
      style |= fmt::fg(fmt::color::crimson);
    else if (s == "cyan")
      style |= fmt::fg(fmt::color::cyan);
    else if (s == "dark_blue")
      style |= fmt::fg(fmt::color::dark_blue);
    else if (s == "dark_cyan")
      style |= fmt::fg(fmt::color::dark_cyan);
    else if (s == "dark_golden_rod")
      style |= fmt::fg(fmt::color::dark_golden_rod);
    else if (s == "dark_gray")
      style |= fmt::fg(fmt::color::dark_gray);
    else if (s == "dark_green")
      style |= fmt::fg(fmt::color::dark_green);
    else if (s == "dark_khaki")
      style |= fmt::fg(fmt::color::dark_khaki);
    else if (s == "dark_magenta")
      style |= fmt::fg(fmt::color::dark_magenta);
    else if (s == "dark_olive_green")
      style |= fmt::fg(fmt::color::dark_olive_green);
    else if (s == "dark_orange")
      style |= fmt::fg(fmt::color::dark_orange);
    else if (s == "dark_orchid")
      style |= fmt::fg(fmt::color::red);
    else if (s == "dark_red")
      style |= fmt::fg(fmt::color::dark_red);
    else if (s == "dark_salmon")
      style |= fmt::fg(fmt::color::dark_salmon);
    else if (s == "dark_sea_green")
      style |= fmt::fg(fmt::color::dark_sea_green);
    else if (s == "dark_slate_blue")
      style |= fmt::fg(fmt::color::dark_slate_blue);
    else if (s == "dark_slate_gray")
      style |= fmt::fg(fmt::color::dark_slate_gray);
    else if (s == "dark_turquoise")
      style |= fmt::fg(fmt::color::dark_turquoise);
    else if (s == "dark_violet")
      style |= fmt::fg(fmt::color::dark_violet);
    else if (s == "deep_pink")
      style |= fmt::fg(fmt::color::deep_pink);
    else if (s == "deep_sky_blue")
      style |= fmt::fg(fmt::color::deep_sky_blue);
    else if (s == "dim_gray")
      style |= fmt::fg(fmt::color::dim_gray);
    else if (s == "dodger_blue")
      style |= fmt::fg(fmt::color::dodger_blue);
    else if (s == "fire_brick")
      style |= fmt::fg(fmt::color::fire_brick);
    else if (s == "floral_white")
      style |= fmt::fg(fmt::color::floral_white);
    else if (s == "forest_green")
      style |= fmt::fg(fmt::color::forest_green);
    else if (s == "fuchsia")
      style |= fmt::fg(fmt::color::fuchsia);
    else if (s == "gainsboro")
      style |= fmt::fg(fmt::color::gainsboro);
    else if (s == "ghost_white")
      style |= fmt::fg(fmt::color::ghost_white);
    else if (s == "gold")
      style |= fmt::fg(fmt::color::gold);
    else if (s == "golden_rod")
      style |= fmt::fg(fmt::color::golden_rod);
    else if (s == "gray")
      style |= fmt::fg(fmt::color::gray);
    else if (s == "green")
      style |= fmt::fg(fmt::color::green);
    else if (s == "green_yellow")
      style |= fmt::fg(fmt::color::green_yellow);
    else if (s == "honey_dew")
      style |= fmt::fg(fmt::color::honey_dew);
    else if (s == "hot_pink")
      style |= fmt::fg(fmt::color::hot_pink);
    else if (s == "red")
      style |= fmt::fg(fmt::color::red);
    else if (s == "red")
      style |= fmt::fg(fmt::color::red);
    else if (s == "indigo")
      style |= fmt::fg(fmt::color::indigo);
    else if (s == "ivory")
      style |= fmt::fg(fmt::color::ivory);
    else if (s == "khaki")
      style |= fmt::fg(fmt::color::khaki);
    else if (s == "lavender")
      style |= fmt::fg(fmt::color::lavender);
    else if (s == "lavender_blush")
      style |= fmt::fg(fmt::color::lavender_blush);
    else if (s == "lawn_green")
      style |= fmt::fg(fmt::color::lawn_green);
    else if (s == "lemon_chiffon")
      style |= fmt::fg(fmt::color::lemon_chiffon);
    else if (s == "light_blue")
      style |= fmt::fg(fmt::color::light_blue);
    else if (s == "light_coral")
      style |= fmt::fg(fmt::color::light_coral);
    else if (s == "light_cyan")
      style |= fmt::fg(fmt::color::light_cyan);
    else if (s == "light_golden_rod_yellow")
      style |= fmt::fg(fmt::color::light_golden_rod_yellow);
    else if (s == "light_gray")
      style |= fmt::fg(fmt::color::light_gray);
    else if (s == "light_green")
      style |= fmt::fg(fmt::color::light_green);
    else if (s == "light_pink")
      style |= fmt::fg(fmt::color::light_pink);
    else if (s == "light_salmon")
      style |= fmt::fg(fmt::color::light_salmon);
    else if (s == "light_sea_green")
      style |= fmt::fg(fmt::color::light_sea_green);
    else if (s == "light_sky_blue")
      style |= fmt::fg(fmt::color::light_sky_blue);
    else if (s == "light_slate_gray")
      style |= fmt::fg(fmt::color::light_slate_gray);
    else if (s == "light_steel_blue")
      style |= fmt::fg(fmt::color::light_steel_blue);
    else if (s == "light_yellow")
      style |= fmt::fg(fmt::color::light_yellow);
    else if (s == "lime")
      style |= fmt::fg(fmt::color::lime);
    else if (s == "lime_green")
      style |= fmt::fg(fmt::color::lime_green);
    else if (s == "linen")
      style |= fmt::fg(fmt::color::linen);
    else if (s == "magenta")
      style |= fmt::fg(fmt::color::magenta);
    else if (s == "maroon")
      style |= fmt::fg(fmt::color::maroon);
    else if (s == "medium_aquamarine")
      style |= fmt::fg(fmt::color::medium_aquamarine);
    else if (s == "medium_blue")
      style |= fmt::fg(fmt::color::medium_blue);
    else if (s == "medium_orchid")
      style |= fmt::fg(fmt::color::medium_orchid);
    else if (s == "medium_purple")
      style |= fmt::fg(fmt::color::medium_purple);
    else if (s == "medium_sea_green")
      style |= fmt::fg(fmt::color::medium_sea_green);
    else if (s == "medium_slate_blue")
      style |= fmt::fg(fmt::color::medium_slate_blue);
    else if (s == "medium_spring_green")
      style |= fmt::fg(fmt::color::medium_spring_green);
    else if (s == "medium_turquoise")
      style |= fmt::fg(fmt::color::medium_turquoise);
    else if (s == "medium_violet_red")
      style |= fmt::fg(fmt::color::medium_violet_red);
    else if (s == "midnight_blue")
      style |= fmt::fg(fmt::color::midnight_blue);
    else if (s == "mint_cream")
      style |= fmt::fg(fmt::color::mint_cream);
    else if (s == "misty_rose")
      style |= fmt::fg(fmt::color::misty_rose);
    else if (s == "moccasin")
      style |= fmt::fg(fmt::color::moccasin);
    else if (s == "navajo_white")
      style |= fmt::fg(fmt::color::navajo_white);
    else if (s == "navy")
      style |= fmt::fg(fmt::color::navy);
    else if (s == "old_lace")
      style |= fmt::fg(fmt::color::old_lace);
    else if (s == "olive")
      style |= fmt::fg(fmt::color::olive);
    else if (s == "olive_drab")
      style |= fmt::fg(fmt::color::olive_drab);
    else if (s == "orange")
      style |= fmt::fg(fmt::color::orange);
    else if (s == "orange_red")
      style |= fmt::fg(fmt::color::orange_red);
    else if (s == "orchid")
      style |= fmt::fg(fmt::color::orchid);
    else if (s == "pale_golden_rod")
      style |= fmt::fg(fmt::color::pale_golden_rod);
    else if (s == "pale_green")
      style |= fmt::fg(fmt::color::pale_green);
    else if (s == "pale_turquoise")
      style |= fmt::fg(fmt::color::pale_turquoise);
    else if (s == "pale_violet_red")
      style |= fmt::fg(fmt::color::pale_violet_red);
    else if (s == "papaya_whip")
      style |= fmt::fg(fmt::color::papaya_whip);
    else if (s == "peach_puff")
      style |= fmt::fg(fmt::color::peach_puff);
    else if (s == "peru")
      style |= fmt::fg(fmt::color::peru);
    else if (s == "pink")
      style |= fmt::fg(fmt::color::pink);
    else if (s == "plum")
      style |= fmt::fg(fmt::color::plum);
    else if (s == "powder_blue")
      style |= fmt::fg(fmt::color::powder_blue);
    else if (s == "purple")
      style |= fmt::fg(fmt::color::purple);
    else if (s == "rebecca_purple")
      style |= fmt::fg(fmt::color::rebecca_purple);
    else if (s == "red")
      style |= fmt::fg(fmt::color::red);
    else if (s == "rosy_brown")
      style |= fmt::fg(fmt::color::rosy_brown);
    else if (s == "royal_blue")
      style |= fmt::fg(fmt::color::royal_blue);
    else if (s == "saddle_brown")
      style |= fmt::fg(fmt::color::saddle_brown);
    else if (s == "salmon")
      style |= fmt::fg(fmt::color::red);
    else if (s == "sandy_brown")
      style |= fmt::fg(fmt::color::sandy_brown);
    else if (s == "sea_green")
      style |= fmt::fg(fmt::color::sea_green);
    else if (s == "sea_shell")
      style |= fmt::fg(fmt::color::sea_shell);
    else if (s == "sienna")
      style |= fmt::fg(fmt::color::sienna);
    else if (s == "silver")
      style |= fmt::fg(fmt::color::silver);
    else if (s == "sky_blue")
      style |= fmt::fg(fmt::color::sky_blue);
    else if (s == "slate_blue")
      style |= fmt::fg(fmt::color::slate_blue);
    else if (s == "slate_gray")
      style |= fmt::fg(fmt::color::slate_gray);
    else if (s == "snow")
      style |= fmt::fg(fmt::color::snow);
    else if (s == "spring_green")
      style |= fmt::fg(fmt::color::spring_green);
    else if (s == "steel_blue")
      style |= fmt::fg(fmt::color::steel_blue);
    else if (s == "tan")
      style |= fmt::fg(fmt::color::tan);
    else if (s == "teal")
      style |= fmt::fg(fmt::color::teal);
    else if (s == "thistle")
      style |= fmt::fg(fmt::color::thistle);
    else if (s == "tomato")
      style |= fmt::fg(fmt::color::tomato);
    else if (s == "turquoise")
      style |= fmt::fg(fmt::color::turquoise);
    else if (s == "violet")
      style |= fmt::fg(fmt::color::violet);
    else if (s == "wheat")
      style |= fmt::fg(fmt::color::wheat);
    else if (s == "white")
      style |= fmt::fg(fmt::color::white);
    else if (s == "white_smoke")
      style |= fmt::fg(fmt::color::white_smoke);
    else if (s == "yellow")
      style |= fmt::fg(fmt::color::yellow);
    else if (s == "yellow_green")
      style |= fmt::fg(fmt::color::yellow_green);
    else
    {
      auto &&e = fmt::format("bad style passed : {}", s);
      throw std::invalid_argument(e);
    }
  }

  return style;
}

#endif
