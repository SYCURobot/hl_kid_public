#include "TemplateFactory.hpp"

#include "TemplateOpenCV.hpp"
#include "TemplateII.hpp"

#include "../FilterFactory.hpp"

namespace Vision {
namespace Filters {

void registerTemplateFilters(FilterFactory * ff) {
  ff->registerBuilder("TemplateOpenCV", [](){return std::unique_ptr<Filter>(new TemplateOpenCV());});
  ff->registerBuilder("TemplateII", [](){return std::unique_ptr<Filter>(new TemplateII());});
}

}
}
