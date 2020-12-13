#include <vector>
#include <any>
#include <string>
#include <sstream>
#include <tuple>
#include <map>
#include <exception>
#include <stdexcept>
#include <iostream>

namespace black::clon
{
  enum struct clon_type
  {
    string,
    boolean,
    number,
    object,
    none
  };

  struct clon
  {
    clon_type type = clon_type::none;
    std::string name;
    std::any val;
  };

  using clons = std::vector<clon>;
  using lst_ref = std::vector<clon> &;
  using str_ref = std::string &;
  using dbl_ref = double &;
  using bol_ref = bool &;

  using lst_cref = const std::vector<clon> &;
  using str_cref = const std::string &;
  using dbl_cref = const double &;
  using bol_cref = const bool &;

  bol_cref cast_bool(const clon &c)
  {
    return std::any_cast<bol_cref>(c);
  }

  bol_ref cast_bool(clon &c)
  {
    return std::any_cast<bol_ref>(c);
  }

  str_cref cast_string(const clon &c)
  {
    return std::any_cast<str_cref>(c);
  }

  str_ref cast_string(clon &c)
  {
    return std::any_cast<str_ref>(c);
  }

  dbl_cref cast_number(const clon &c)
  {
    return std::any_cast<dbl_cref>(c);
  }

  dbl_ref cast_number(clon &c)
  {
    return std::any_cast<dbl_ref>(c);
  }

  lst_cref cast_object(const clon &c)
  {
    return std::any_cast<lst_cref>(c);
  }

  lst_ref cast_object(clon &c)
  {
    return std::any_cast<lst_ref>(c);
  }

  inline void to_string_basic(
      std::stringstream &ss,
      const clon &c);

  inline void to_string_bool(
      std::stringstream &ss,
      const clon &c)
  {
    ss << cast_bool(c) ? "true" : "false";
  }

  inline void to_string_string(
      std::stringstream &ss,
      const clon &c)
  {
    ss << '"' << cast_string(c) << '"';
  }

  inline void to_string_number(
      std::stringstream &ss,
      const clon &c)
  {
    ss << cast_number(c);
  }

  inline void to_string_object(
      std::stringstream &ss,
      const clon &c)
  {
    for (auto &&item : cast_object(c))
      to_string_basic(ss, item);
  }
  
  inline void to_string_basic(
      std::stringstream &ss,
      const clon &c)
  {
    ss << "(" << c.name;

    switch (c.type)
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

    ss << ")";
  }

  inline std::string to_string(const clon &c)
  {
    std::stringstream ss;
    to_string_basic(ss, c);
    return ss.str();
  }

  // std::string::const_iterator remove_blank(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   while (b != e and (*b == '\n' or
  //                      *b == ' ' or
  //                      *b == '\t' or
  //                      *b == '\r'))
  //     b++;

  //   return b;
  // }

  // clon_type next_could_be(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   if (b != e)
  //     if (*b == '"')
  //       return clon_type::string;
  //     else if (*b == '(')
  //       return clon_type::object;
  //     else if (*b == 't')
  //       return clon_type::boolean;
  //     else if (*b == 'f')
  //       return clon_type::boolean;
  //     else if (*b == '-')
  //       return clon_type::list;
  //     else if ('0' <= *b and *b <= '9')
  //       return clon_type::number;

  //   return clon_type::none;
  // }

  // std::tuple<clon, std::string::const_iterator>
  // parse_string(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   auto sb = b;

  //   if (b != e)
  //     if (*b == '"')
  //     {
  //       b++;
  //       while (b != e and *b != '"')
  //         b++;
  //       if (*b == '"')
  //       {
  //         clon s;
  //         s = std::string(sb, b);
  //         return {s, b};
  //       }
  //     }

  //   throw std::invalid_argument(
  //       "a character '\"' is expected");
  // }

  // std::tuple<clon, std::string::const_iterator>
  // parse_number(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   auto sb = b;

  //   if (b != e)
  //     if ('0' <= *b and *b <= '9')
  //     {
  //       while ('0' <= *b and *b <= '9')
  //         b++;

  //       clon n;
  //       std::string ns(sb, b);
  //       n = std::stod(ns);
  //       return {n, b};
  //     }

  //   throw std::invalid_argument(
  //       "a character in '[0-9]' is expected");
  // }

  // std::tuple<clon, std::string::const_iterator>
  // parse_bool(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   if (b != e)
  //   {
  //     if (*b == 't' and
  //         (b + 1) != e and *(b + 1) == 'r' and
  //         (b + 2) != e and *(b + 2) == 'u' and
  //         (b + 3) != e and *(b + 3) == 'e')

  //     {
  //       clon bo;
  //       bo = true;
  //       return {bo, b + 4};
  //     }
  //     else if (*b == 'f' and
  //              (b + 1) != e and *(b + 1) == 'a' and
  //              (b + 2) != e and *(b + 2) == 'l' and
  //              (b + 3) != e and *(b + 3) == 's' and
  //              (b + 4) != e and *(b + 4) == 'e')

  //     {
  //       clon bo;
  //       bo = false;
  //       return {bo, b + 5};
  //     }
  //   }

  //   throw std::invalid_argument(
  //       "'true' or 'false' is expected");
  // }

  // std::tuple<clon, std::string::const_iterator>
  // parse_list(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  // }

  // std::string object_name(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   auto sb = b;

  //   while ('a' <= *b and *b <= 'z')
  //     b++;

  //   return std::string(sb, b);
  // }

  // bool end_object(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   return b != e and *b == ')';
  // }

  // std::tuple<clon, std::string::const_iterator, std::string>
  // parse_object(
  //     std::string::const_iterator b,
  //     std::string::const_iterator e)
  // {
  //   auto sb = b;

  //   if (*b == '(')
  //     while (b != e and *b != ')')
  //     {
  //       b = remove_blank(b, e);
  //       auto nb = b;

  //       if ('a' <= *b and *b <= 'z')
  //       {
  //         std::string name = object_name(b, e);
  //         b = remove_blank(b, e);

  //         switch (next_could_be(b, e))
  //         {
  //         case clon_type::boolean:
  //           auto &&res = parse_bool(b, e);
  //           b = remove_blank(std::get<1>(res), e);

  //           if (end_object(b, e))
  //           {
  //             b++;

  //             if (std::get<0>(res).is_bool())
  //               return {std::get<0>(res), b, name};
  //           }

  //           break;
  //         case clon_type::number:
  //           auto &&res = parse_number(b, e);
  //           b = remove_blank(std::get<1>(res), e);

  //           if (end_object(b, e))
  //           {
  //             b++;

  //             if (std::get<0>(res).is_number())
  //               return {std::get<0>(res), b, name};
  //           }

  //           break;
  //         case clon_type::string:
  //           auto &&res = parse_string(b, e);
  //           b = remove_blank(std::get<1>(res), e);

  //           if (end_object(b, e))
  //           {
  //             b++;

  //             if (std::get<0>(res).is_number())
  //               return {std::get<0>(res), b, name};
  //           }

  //           break;
  //         case clon_type::list:
  //           while (b != e and *b == '-')
  //           {

  //           }
  //           break;
  //         case clon_type::object:
  //           break;
  //         case clon_type::none:
  //           break;
  //         }

  //         //return {c, b, name};
  //       }
  //     }
  // }

  // clon parse(const std::string &str)
  // {
  //   auto b = str.begin();
  //   auto e = str.end();

  //   if (next_could_be(b, e) == clon_type::object)
  //   {
  //     clon c;
  //     auto &&res = parse_object(b, e);
  //     auto &&cc = std::get<0>(res);
  //     auto &&cur = std::get<1>(res);
  //     auto &&name = std::get<2>(res);

  //     if (cur == e)
  //     {
  //       c[name] = cc;
  //       return c;
  //     }
  //   }

  //   throw std::invalid_argument(
  //       "a character '(' is expected");
  // }

} // namespace black::clon
