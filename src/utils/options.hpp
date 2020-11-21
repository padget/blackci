#ifndef __black_options_hpp__
#define __black_options_hpp__

#include <string>
#include <string_view>
#include <optional>
#include <boost/program_options.hpp>

namespace black
{
  namespace po = boost::program_options;

  struct option
  {
    std::string name;
    std::string description;
    std::string defaultv;
    bool required;
  };

  class options
  {
    po::options_description opts;
    int argc;
    char **argv;
    po::variables_map vm;

  public:
    explicit options(
        const std::string &name,
        int _argc,
        char **_argv)
        : opts{name},
          argc{_argc},
          argv{_argv} {}

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
      auto pname = name.data();
      auto pdesc = description.data();
      auto value = po::value<type_t>();

      // TODO surveiller si besoins
      // d'une réaffectation ou non
      if (required)
        value->required();

      opts.add_options()(pname, value, pdesc);
    }

    inline void parse()
    {
      po::store(po::parse_command_line(argc, argv, opts), vm);
      po::notify(vm);
    }

    inline int count(
        const std::string &key)
    {
      return vm.count(key);
    }

    inline bool exists(
        const std::string &key)
    {
      return count(key) == 1;
    }

    

    template <typename type_t>
    std::optional<type_t> get(
        const std::string &key)
    {
      return vm[key].as<type_t>();
    }

    inline auto get_str(
        const std::string &key)
    {
      return get<std::string>(key);
    }

    inline auto get_ull(
        const std::string &key)

    {
      return get<unsigned long long>(key);
    }

    inline const po::options_description &
    description() const
    {
      return opts;
    }
  };
} // namespace black

#endif
