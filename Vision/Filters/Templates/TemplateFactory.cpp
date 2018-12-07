#include "TemplateFactory.hpp"

#include "TemplateOpenCV.hpp"
#include "ObjectByII.hpp"
#include "ObjectByDNN.hpp"

#include "../FilterFactory.hpp"

namespace Vision {
namespace Filters {

void registerTemplateFilters(FilterFactory * ff) {
  ff->registerBuilder("TemplateOpenCV", [](){return std::unique_ptr<Filter>(new TemplateOpenCV());});
  ff->registerBuilder("ObjectByII", [](){return std::unique_ptr<Filter>(new ObjectByII());});
  ff->registerBuilder("ObjectByDNN", [](){return std::unique_ptr<Filter>(new ObjectByDNN());});
}

}
}
