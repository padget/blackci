#ifndef __black_clon_hpp__
#define __black_clon_hpp__

#include <vector>
#include <any>
#include <variant>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fmt/format.h>
#include <boost/tokenizer.hpp>
#include <type_traits>

namespace black::clon
{
  enum struct clon_type : int
  {
    none = 0,
    boolean = 1,
    number = 2,
    string = 3,
    object = 4,
  };

  struct clon;

  using number = double;
  using boolean = bool;
  using string = std::string;
  using object = std::vector<clon>;
  using clon_value = std::variant<
    std::monostate, boolean,
    number, string, object>;;

  struct clon
  {
    std::string name;
    clon_value val;
  };


  clon& undefined()
  {
    static clon c;
    return c;
  }

  clon_type type(const clon& c)
  {
    switch (c.val.index())
    {
    case 0:
      return clon_type::none;
    case 1:
      return clon_type::boolean;
    case 2:
      return clon_type::number;
    case 3:
      return clon_type::string;
    case 4:
      return clon_type::object;
    default:
      return clon_type::none;
    }
  }

  bool is_none(const clon& c)
  {
    return type(c) == clon_type::none;
  }

  bool is_bool(const clon& c)
  {
    return type(c) == clon_type::boolean;
  }

  bool is_number(const clon& c)
  {
    return type(c) == clon_type::number;
  }

  bool is_string(const clon& c)
  {
    return type(c) == clon_type::string;
  }

  bool is_object(const clon& c)
  {
    return type(c) == clon_type::object;
  }

  using lst_ref = object&;
  using str_ref = string&;
  using dbl_ref = number&;
  using bol_ref = boolean&;

  using lst_cref = const object&;
  using str_cref = const string&;
  using dbl_cref = const number&;
  using bol_cref = const boolean&;

  bol_cref as_bool(const clon& c)
  {
    return std::get<1>(c.val);
  }

  bol_ref as_bool(clon& c)
  {
    return std::get<1>(c.val);
  }

  str_cref as_string(const clon& c)
  {
    return std::get<string>(c.val);
  }

  str_ref as_string(clon& c)
  {
    return std::get<string>(c.val);
  }

  dbl_cref as_number(const clon& c)
  {
    return std::get<2>(c.val);
  }

  dbl_ref as_number(clon& c)
  {
    return std::get<2>(c.val);
  }

  lst_cref as_object(const clon& c)
  {
    return std::get<4>(c.val);
  }

  lst_ref as_object(clon& c)
  {
    return std::get<4>(c.val);
  }
  namespace detail
  {
    inline void to_string_basic(
      std::stringstream& ss,
      const clon& c);

    inline void to_string_bool(
      std::stringstream& ss,
      const clon& c)
    {
      ss << (as_bool(c) ? "true" : "false");
    }

    inline void to_string_string(
      std::stringstream& ss,
      const clon& c)
    {
      ss << '"' << as_string(c) << '"';
    }

    inline void to_string_number(
      std::stringstream& ss,
      const clon& c)
    {
      ss << as_number(c);
    }

    inline void to_string_object(
      std::stringstream& ss,
      const clon& c)
    {
      for (auto&& item : as_object(c))
        to_string_basic(ss, item);
    }

    inline void to_string_basic(
      std::stringstream& ss,
      const clon& c)
    {
      ss << '(' << c.name << ' ';

      switch (type(c))
      {
      case clon_type::boolean:
        to_string_bool(ss, c);
        break;
      case clon_type::number:
        to_string_number(ss, c);
        break;
      case clon_type::string:
        to_string_string(ss, c);
        break;
      case clon_type::object:
        to_string_object(ss, c);
        break;
      case clon_type::none:
        break;
      }

      ss << ')';
    }
  }

  std::string to_string(const clon& c)
  {
    std::stringstream ss;
    detail::to_string_basic(ss, c);
    return ss.str();
  }

  class expected_character : public std::invalid_argument
  {
  public:
    expected_character(const std::string& chars)
      : std::invalid_argument(
        fmt::format("expected characters : {}", chars))
    {
    }
  };

  class malformed_path : public std::invalid_argument
  {
  public:
    malformed_path(const std::string& reason)
      : std::invalid_argument(
        fmt::format("malformed path : {}", reason))
    {
    }
  };

  class malformed_number : public malformed_path
  {
  public:
    malformed_number(const std::string& path)
      : malformed_path(
        fmt::format("malformed number '{}'", path))
    {
    }
  };

  class malformed_name : public malformed_path
  {
  public:
    malformed_name(const std::string& path)
      : malformed_path(
        fmt::format("malformed name '{}'", path))
    {
    }
  };

  namespace detail
  {
    clon_type next_could_be(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      if (b != e)
      {
        if (*b == '"')
          return clon_type::string;
        else if (*b == '(')
          return clon_type::object;
        else if (*b == 't')
          return clon_type::boolean;
        else if (*b == 'f')
          return clon_type::boolean;
        else if ('0' <= *b and *b <= '9')
          return clon_type::number;
      }
      return clon_type::none;
    }

    std::string::const_iterator remove_blank(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      while (b != e and (*b == '\n' or
        *b == ' ' or
        *b == '\t' or
        *b == '\r'))
        b++;

      return b;
    }

    std::string object_name(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      auto sb = b;

      while ('a' <= *b and *b <= 'z')
        b++;

      return std::string(sb, b);
    }

    template<typename type_t>
    struct parse_result
    {
      type_t val;
      std::string::const_iterator i;
    };

    bool between(
      const char min,
      const char c,
      const char max)
    {
      return min <= c and c <= max;
    }

    std::string::const_iterator parse_until(
      std::string::const_iterator b,
      std::string::const_iterator e,
      const char c)
    {
      while (b != e and *b != c)
        b++;

      if (b == e)
        throw expected_character("'\"'");

      return b;
    }

    std::string::const_iterator parse_blank(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      while (b != e and (*b == ' ' or
        *b == '\t' or
        *b == '\n' or
        *b == '\r'))
        b++;
      return b;
    }

    parse_result<string> parse_string(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      if (b != e and *b == '"')
      {
        auto sb = b;
        b = parse_until(b + 1, e, '"');
        return { std::string(sb + 1, b), b + 1 };
      }

      throw expected_character("'\"'");
    }

    parse_result<number> parse_number(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      if (b != e and between('0', *b, '9'))
      {
        auto sb = b;

        while (b != e and between('0', *b, '9'))
          b++;

        return { std::stod(std::string(sb, b)), b };
      }

      throw expected_character("[0-9]");
    }

    parse_result<boolean> parse_bool(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      if (b != e and *b == 't' and
        (b + 1) != e and *(b + 1) == 'r' and
        (b + 2) != e and *(b + 2) == 'u' and
        (b + 3) != e and *(b + 3) == 'e')
        return { true, b + 4 };
      else if (b != e and *b == 'f' and
        (b + 1) != e and *(b + 1) == 'a' and
        (b + 2) != e and *(b + 2) == 'l' and
        (b + 3) != e and *(b + 3) == 's' and
        (b + 4) != e and *(b + 4) == 'e')
        return { false, b + 5 };

      throw expected_character("true|false");
    }

    parse_result<clon> parse_one(
      std::string::const_iterator b,
      std::string::const_iterator e);

    parse_result<object> parse_object(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      if (b != e and *b == '(')
      {
        object cls;

        while (b != e and *b == '(')
        {
          auto&& res = parse_one(b, e);
          cls.push_back(res.val);
          b = res.i;
          b = parse_blank(b, e);
        }

        return { cls, b };
      }

      throw expected_character("'('");
    }

    parse_result<clon> parse_one(
      std::string::const_iterator b,
      std::string::const_iterator e)
    {
      if (b != e and *b == '(')
      {
        clon c;
        b = parse_blank(b + 1, e);
        c.name = object_name(b, e);
        std::advance(b, c.name.size());
        b = parse_blank(b, e);

        switch (next_could_be(b, e))
        {
        case clon_type::boolean:
        {
          auto&& bl = parse_bool(b, e);
          b = bl.i;
          c.val = bl.val;
          break;
        }
        case clon_type::number:
        {
          auto&& nb = parse_number(b, e);
          b = nb.i;
          c.val = nb.val;
          break;
        }
        case clon_type::string:
        {
          auto&& st = parse_string(b, e);
          b = st.i;
          c.val = st.val;
          break;
        }
        case clon_type::object:
        {
          auto&& ob = parse_object(b, e);
          b = ob.i;
          c.val = ob.val;
          break;
        }
        case clon_type::none:
          throw expected_character("(|true|false|'\"'|[0-9]");
        }

        b = parse_blank(b, e);

        if (b != e and *b == ')')
          return { std::move(c), b + 1 };

        throw expected_character(")");
      }

      throw expected_character("(");
    }
  } // namespace detail

  clon parse(
    std::string::const_iterator b,
    std::string::const_iterator e)
  {
    auto&& res = detail::parse_one(b, e);
    b = res.i;
    b = detail::parse_blank(b, e);

    if (res.i == e)
      return res.val;

    throw std::exception();
  }

  namespace detail
  {
    struct path
    {
      std::string p;
      std::size_t idx = 0;
    };

    using paths = std::vector<path>;

    std::size_t count(
      const std::string& str,
      const char c)
    {
      return std::count(str.begin(), str.end(), c);
    }

    bool is_digit(const char c)
    {
      return '0' <= c and c <= '9';
    }

    bool is_number(const std::string& s)
    {
      return !s.empty() and
        std::all_of(s.begin(), s.end(), is_digit);
    }

    bool is_lower(const char c)
    {
      return between('a', c, 'z');
    }

    bool is_name(const std::string& s)
    {
      return !s.empty() and
        std::all_of(s.begin(), s.end(), is_lower);
    }

    path to_path(const std::string& spath)
    {
      using sep = boost::char_separator<char>;
      using tokenizer = boost::tokenizer<sep>;

      auto toks = tokenizer(spath, sep(":"));
      auto cnt = std::distance(toks.begin(), toks.end());

      if (cnt > 2)
        throw malformed_path(
          fmt::format("more than once ':' in path '{}'", spath));

      if (cnt == 0)
      {
        if (not is_name(spath))
          throw malformed_name(spath);

        return { spath, 0 };
      }
      else
      {
        auto tok = toks.begin();
        auto end = toks.end();

        if (tok == end)
          throw malformed_path("empty path");

        auto pth = *tok;

        if (not is_name(pth))
          throw malformed_name(pth);

        tok++;
        std::size_t idx = 0;

        if (tok != end)
        {
          if (not is_number(*tok))
            throw malformed_number(*tok);
          else
            idx = std::stoull(*tok);
        }

        return { pth, idx };
      }
    }

    paths to_paths(const std::string& spath)
    {
      using sep = boost::char_separator<char>;
      using tokenizer = boost::tokenizer<sep>;

      paths pths;

      for (auto&& tok : tokenizer(spath, sep(".")))
        pths.push_back(to_path(tok));

      return pths;
    }

    const clon& get(
      const path& pth,
      const clon& c)
    {
      if (is_object(c))
      {
        std::size_t cnt = 0;

        for (const clon& sub : as_object(c))
          if (sub.name == pth.p)
          {
            if (cnt == pth.idx)
              return sub;
            else
              cnt++;
          }
      }

      return undefined();
    }

    const clon& get(
      paths::const_iterator b,
      paths::const_iterator e,
      const clon& c)
    {
      if (b != e and not is_none(c))
      {
        const clon& sub = get(*b, c);
        b++;
        return get(b, e, sub);
      }
      else if (b == e and is_none(c))
        return undefined();
      else if (b == e and not is_none(c))
        return c;
      else
        return undefined();
    }
  }// namespace detail

  const clon& get(
    const std::string& path,
    const clon& c)
  {
    detail::paths&& pths =
      detail::to_paths(path);
    return detail::get(
      pths.begin(), pths.end(), c);
  }

  const string& get_string(
    const std::string& pth,
    const clon& c)
  {
    return as_string(get(pth, c));
  }

  const number& get_number(
    const std::string& pth,
    const clon& c)
  {
    return as_number(get(pth, c));
  }

  const boolean& get_bool(
    const std::string& pth,
    const clon& c)
  {
    return as_bool(get(pth, c));
  }

  const object& get_object(
    const std::string& pth,
    const clon& c)
  {
    return as_object(get(pth, c));
  }

  void set(
    const std::string& pth, clon& c,
    const clon_value& value)
  {
    clon& sub = const_cast<clon&>(get(pth, c));
    sub.val = value;
  }
} // namespace black::clon

#endif
