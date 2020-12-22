#ifndef __black_clon_hpp__
#define __black_clon_hpp__

#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <fmt/format.h>
#include <istream>

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


  str_cref as_string(const clon& c)
  {
    return std::get<string>(c.val);
  }

  dbl_cref as_number(const clon& c)
  {
    return std::get<2>(c.val);
  }


  lst_cref as_object(const clon& c)
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

  class expected_character
    : public std::invalid_argument
  {
  public:
    expected_character(
      std::string_view chars)
      : std::invalid_argument(
        fmt::format(
          "expected characters : {}", chars))
    {}
  };

  class malformed_path
    : public std::invalid_argument
  {
  public:
    malformed_path(
      std::string_view reason)
      : std::invalid_argument(
        fmt::format(
          "malformed path : {}", reason))
    {}
  };

  class malformed_number
    : public malformed_path
  {
  public:
    malformed_number(
      std::string_view path)
      : malformed_path(
        fmt::format(
          "malformed number '{}'", path))
    {}
  };

  class malformed_name
    : public malformed_path
  {
  public:
    malformed_name(
      std::string_view path)
      : malformed_path(
        fmt::format(
          "malformed name '{}'", path))
    {}
  };

  class unreachable_path :
    public std::invalid_argument
  {
  public:
    unreachable_path(
      std::string_view pth)
      : std::invalid_argument(
        fmt::format(
          "unreachable path : {}", pth))
    {}
  };

  namespace detail
  {
    template<typename char_iterator>
    clon_type next_could_be(
      char_iterator b,
      char_iterator e)
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

    template<typename char_iterator>
    char_iterator remove_blank(
      char_iterator b,
      char_iterator e)
    {
      while (b != e and (*b == '\n' or
        *b == ' ' or
        *b == '\t' or
        *b == '\r'))
        std::advance(b, 1);

      return b;
    }

    template<typename char_iterator>
    std::string object_name(
      char_iterator b,
      char_iterator e)
    {
      auto sb = b;

      while ('a' <= *b and *b <= 'z')
        std::advance(b, 1);

      return std::string(sb, b);
    }

    template<typename type_t, typename iterator>
    struct parse_result
    {
      type_t val;
      iterator i;
    };

    bool between(
      const char min,
      const char c,
      const char max)
    {
      return min <= c and c <= max;
    }

    template<typename char_iterator>
    char_iterator parse_until(
      char_iterator b,
      char_iterator e,
      const char c)
    {
      while (b != e and *b != c)
        std::advance(b, 1);

      if (b == e)
        throw expected_character(
          fmt::format("'{}'", c));

      return b;
    }

    template<typename char_iterator>
    char_iterator parse_blank(
      char_iterator b,
      char_iterator e)
    {
      while (b != e and (*b == ' ' or
        *b == '\t' or
        *b == '\n' or
        *b == '\r'))
        std::advance(b, 1);
      return b;
    }

    template<typename char_iterator>
    parse_result<string, char_iterator> parse_string(
      char_iterator b,
      char_iterator e)
    {
      if (b != e and *b == '"')
      {
        auto sb = b;
        b = parse_until(std::next(b), e, '"');
        return { std::string(std::next(sb), b), std::next(b) };
      }

      throw expected_character("'\"'");
    }

    template<typename char_iterator>
    parse_result<number, char_iterator> parse_number(
      char_iterator b,
      char_iterator e)
    {
      if (b != e and between('0', *b, '9'))
      {
        auto sb = b;

        while (b != e and between('0', *b, '9'))
          std::advance(b, 1);

        return { std::stod(std::string(sb, b)), b };
      }

      throw expected_character("[0-9]");
    }

    template<typename char_iterator>
    parse_result<boolean, char_iterator> parse_bool(
      char_iterator b,
      char_iterator e)
    {
      auto b0 = b;
      auto b1 = b0 != e ? std::next(b0) : e;
      auto b2 = b1 != e ? std::next(b1) : e;
      auto b3 = b2 != e ? std::next(b2) : e;
      auto b4 = b3 != e ? std::next(b3) : e;
      auto b5 = b4 != e ? std::next(b4) : e;

      if (b0 != e and *b0 == 't' and
        b1 != e and *b1 == 'r' and
        b2 != e and *b2 == 'u' and
        b3 != e and *b3 == 'e')
        return { true, b4 };
      else if (b0 != e and *b0 == 'f' and
        b1 != e and *b1 == 'a' and
        b2 != e and *b2 == 'l' and
        b3 != e and *b3 == 's' and
        b4 != e and *b4 == 'e')
        return { false, b5 };

      throw expected_character("true|false");
    }

    template<typename char_iterator>
    parse_result<clon, char_iterator> parse_one(
      char_iterator b,
      char_iterator e);

    template<typename char_iterator>
    parse_result<object, char_iterator> parse_object(
      char_iterator b,
      char_iterator e)
    {
      if (b == e or *b != '(')
        throw expected_character("'('");

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

    template<typename char_iterator>
    parse_result<clon, char_iterator> parse_one(
      char_iterator b,
      char_iterator e)
    {
      if (b == e or *b != '(')
        throw expected_character("'('");

      clon c;
      b = parse_blank(std::next(b), e);
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
        return { std::move(c), std::next(b) };

      throw expected_character(")");
    }

    template<typename char_iterator>
    clon parse(
      char_iterator b,
      char_iterator e)
    {
      auto&& res = detail::parse_one(b, e);
      b = res.i;
      b = detail::parse_blank(b, e);

      if (res.i == e)
        return res.val;

      // TODO error a définir
      throw std::exception();
    }
  } // namespace detail


  clon parse(std::string_view s)
  {
    return detail::parse(
      s.begin(), s.end());
  }

  template<typename ... args>
  clon parse_fmt(
    std::string_view pattern, args&&... as)
  {
    return parse(fmt::format(pattern, as...));
  }

  clon parse_stream(
    std::istream& s)
  {
    std::istreambuf_iterator<char> in(s);
    std::istreambuf_iterator<char> eos;
    return detail::parse(in, eos);
  }

  namespace detail
  {
    struct path
    {
      std::string_view p;
      std::size_t idx = 0;
    };

    using paths = std::vector<path>;

    std::size_t count(
      std::string_view str,
      const char c)
    {
      return std::count(str.begin(), str.end(), c);
    }

    bool is_digit(const char c)
    {
      return '0' <= c and c <= '9';
    }

    bool is_number(std::string_view s)
    {
      return !s.empty() and
        std::all_of(s.begin(), s.end(), is_digit);
    }

    bool is_lower(const char c)
    {
      return between('a', c, 'z');
    }

    bool is_name(std::string_view s)
    {
      return !s.empty() and
        std::all_of(s.begin(), s.end(), is_lower);
    }

    struct tokenizer
    {
      std::string_view::const_iterator b;
      std::string_view::const_iterator e;
      const char sep;
    };

    std::size_t count(const tokenizer& t)
    {
      return std::count(t.b, t.e, t.sep);
    }

    std::string_view next_token(tokenizer& t)
    {
      auto b = t.b;

      while (t.b != t.e)
        if (*(t.b) == t.sep)
        {
          std::advance(t.b, 1);
          break;
        }
        else
          std::advance(t.b, 1);

      if (t.b == t.e)
        return std::string_view(b, t.b);
      else
        return std::string_view(b, std::prev(t.b));
    }

    std::size_t to_size(std::string_view v)
    {
      std::size_t size = v.size();

      switch (size)
      {
      case 0: throw std::exception();
      case 1:
        return (v.back() - '0');
      default:
        std::string_view v{ v.begin(), std::prev(v.end()) };
        std::size_t ten = 10 * to_size(v);
        return (v.back() - '0') + ten;
      }
    }

    path to_path(std::string_view spath)
    {
      auto toks = tokenizer(spath.begin(), spath.end(), ':');
      auto cnt = count(toks);

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
        auto pth = next_token(toks);

        if (pth.size() == 0)
          throw malformed_path("empty path");

        if (not is_name(pth))
          throw malformed_name(pth);

        auto nb = next_token(toks);
        std::size_t idx = 0;

        if (nb.size() != 0)
        {
          if (not is_number(nb))
            throw malformed_number(nb);
          else
            idx = to_size(nb);
        }

        return { pth, idx };
      }
    }

    paths to_paths(std::string_view spath)
    {
      paths pths;
      tokenizer toks(spath.begin(), spath.end(), '.');
      std::string_view tok;

      while ((tok = next_token(toks)).size() > 0)
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
        std::advance(b, 1);
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
    std::string_view path,
    const clon& c)
  {
    detail::paths&& pths =
      detail::to_paths(path);
    return detail::get(
      pths.begin(), pths.end(), c);
  }

  const string& get_string(
    std::string_view pth,
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

    if (is_none(sub))
      throw unreachable_path(pth);

    sub.val = value;
  }


} // namespace black::clon

#endif
