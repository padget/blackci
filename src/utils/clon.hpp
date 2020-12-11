#include <vector>
#include <any>
#include <string>
#include <sstream>
#include <tuple>
#include <map>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace black::clon
{
  enum struct clon_type
  {
    string,  // == std::string
    boolean, // == bool
    number,  // == double
    object,  // == std::map<std::string, clon>
    none     // == type non défini.
  };

  struct clon
  {
    using map_ref = std::map<std::string, std::vector<clon>> &;
    using lst_ref = std::vector<clon> &;
    using str_ref = std::string &;
    using dbl_ref = double &;
    using bol_ref = bool &;

    using map_cref = const std::map<std::string, std::vector<clon>> &;
    using lst_cref = const std::vector<clon> &;
    using str_cref = const std::string &;
    using dbl_cref = const double &;
    using bol_cref = const bool &;

    std::any val;
    clon_type type = clon_type::none;

    inline bool is_none() const
    {
      return type == clon_type::none;
    }

    inline bool is_defined() const
    {
      return not is_none();
    }

    inline bool is_number() const
    {
      return type == clon_type::number;
    }

    inline bool is_string() const
    {
      return type == clon_type::string;
    }

    inline bool is_object() const
    {
      return type == clon_type::object;
    }

    inline bool is_bool() const
    {
      return type == clon_type::boolean;
    }

    str_ref str()
    {
      return std::any_cast<str_ref>(val);
    }

    str_cref str() const
    {
      return std::any_cast<str_cref>(val);
    }

    dbl_ref dbl()
    {
      return std::any_cast<dbl_ref>(val);
    }

    dbl_cref dbl() const
    {
      return std::any_cast<dbl_cref>(val);
    }

    map_ref map()
    {
      return std::any_cast<map_ref>(val);
    }

    map_cref map() const
    {
      return std::any_cast<map_cref>(val);
    }

    bol_ref bol()
    {
      return std::any_cast<bol_ref>(val);
    }

    bol_cref bol() const
    {
      return std::any_cast<bol_cref>(val);
    }

    clon &operator[](
        const std::string &name)
    {
      if (is_none())
      {
        val = std::map<std::string, std::vector<clon>>();
        clon item;
        map()[name].push_back(item);
        type = clon_type::object;
      }

      if (is_object())
      {
        
        return;
      }

      throw std::invalid_argument(
          "'operator[]' can't "
          "be used if clon is "
          "not none or object");
    }

    const clon &operator[](
        const std::string &name) const
    {
      if (is_object())
        return map().at(name);

      throw std::invalid_argument(
          "'operator[]' can't "
          "be used if clon is "
          "not none or object");
    }

    clon &operator=(const std::string &s)
    {
      if (is_none() or is_string())
      {
        type = clon_type::string;
        val = s;
      }

      return *this;
    }

    clon &operator=(const double &n)
    {
      if (is_none() or is_number())
      {
        type = clon_type::number;
        val = n;
      }

      return *this;
    }

    clon &operator=(const clon &o)
    {
      if (this != &o)
      {
        val = o.val;
        type = o.type;
      }

      return *this;
    }

    std::string to_string()
    {
      std::stringstream ss;
      switch (type)
      {
      case clon_type::none:
        break;
      case clon_type::boolean:
        ss << (bol() ? "true" : "false");
        break;
      case clon_type::number:
        ss << dbl();
        break;
      case clon_type::string:
        ss << "\"" << str() << "\"";
        break;
      case clon_type::object:
        for (auto &&item : map())
          if (not item.second.is_none())
            ss << "(" << item.first << ' ' << item.second.to_string() << ")";
        break;
      }

      return ss.str();
    }

    

    bool contains(const std::string &name) const
    {
      if (is_object())
        return not map().count(name) == 0;
      else
        return false;
    }
  };

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
