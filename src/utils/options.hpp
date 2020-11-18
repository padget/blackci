#ifndef __black_options_hpp__
#define __black_options_hpp__

#include <string>
#include <string_view>
#include <boost/program_options.hpp>

namespace black
{
  struct option
  {
    std::string name;
    std::string description;
    std::string defaultv;
    bool required;
  };

  class options
  {
    boost::program_options::options_description opts;

  public:
    explicit options(const std::string &name) : opts{name} {}

    template <typename type_t = void>
    void add(const std::string &name,
             const std::string &description)
    {

      if constexpr (std::is_same_v<type_t, void>)
        opts.add_options()(name.data(), description.data());
      else
        add<type_t>(name, description, false);
    }

    template <typename type_t>
    void add(const std::string &name,
             const std::string &description,
             bool required)
    {
      if (required)
        opts.add_options()(name.data(), boost::program_options::value<type_t>().required(), description.data());
      else
        opts.add_options()(name.data(), boost::program_options::value<type_t>(), description.data());
    }

    inline const boost::program_options::options_description &get() const
    {
      return opts;
    }
  };
} // namespace black

#endif
