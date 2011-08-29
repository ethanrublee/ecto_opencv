#include <ecto/ecto.hpp>
#include <ecto/serialization/registry.hpp>
#include <ecto/serialization/cell.hpp>

#include "highgui.h"

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/format.hpp>

namespace boost
{
  namespace serialization
  {
    template<class Archive>
    void
    save(Archive & ar, const cv::Mat & m, const unsigned int version)
    {
      int type = m.type();
      ar & m.rows;
      ar & m.cols;
      ar & type;
      const uchar * data = m.data, *end = m.dataend;
      ar & boost::serialization::make_binary_object(const_cast<uchar*>(data), size_t(end - data));
    }

    template<class Archive>
    void
    load(Archive & ar, cv::Mat & m, const unsigned int version)
    {
      int rows, cols, type;
      ar & rows;
      ar & cols;
      ar & type;
      if (rows > 0 && cols > 0)
      {
        m.create(rows, cols, type);
        uchar * data = m.data, *end = m.dataend;
        ar & boost::serialization::make_binary_object(data, end - data);
      }
      else
      {
        std::cout << "bad matrix" << std::endl;
      }
    }
  } // namespace serialization
} // namespace boost
BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat);
ECTO_REGISTER_SERIALIZERS(cv::Mat);

ECTO_DEFINE_MODULE(highgui)
{
  boost::python::enum_<ecto_opencv::Record::RecordCommands>("RecordCommands").value("START", ecto_opencv::Record::START).value(
      "RESUME", ecto_opencv::Record::RESUME).value("PAUSE", ecto_opencv::Record::PAUSE).value("STOP",
                                                                                              ecto_opencv::Record::STOP).export_values();
}
