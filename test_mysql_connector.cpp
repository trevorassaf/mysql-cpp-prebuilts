#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <mysqlx/xdevapi.h>

using V = mysqlx::abi2::r0::common::Value;

void PrintRow(mysqlx::Row *row)
{
  assert(row);

  V &value = reinterpret_cast<V &>(row->get(0));
  //const mysqlx::abi2::r0::common::Value &common_value = (const mysqlx::abi2::r0::common::Value &)value;
  size_t index = value.get_uint();

  value = reinterpret_cast<V &>(row->get(1));
  const std::u16string &wide_name = value.get_ustring();
  const std::string &name = std::string(reinterpret_cast<const char *>(wide_name.c_str()), wide_name.size() * 2);

  value = reinterpret_cast<V &>(row->get(2));
  const std::u16string &wide_date = value.get_ustring();
  const std::string &date = std::string(reinterpret_cast<const char *>(wide_date.c_str()), wide_date.size() * 2);

  value = reinterpret_cast<V &>(row->get(3));
  double ceil = value.get_double();

  value = reinterpret_cast<V &>(row->get(4));
  double floor = value.get_double();

  std::cout << "Row: <index=" << index
            << ", name=" << name
            << ", date=" << date
            << ", ceil=" << ceil
            << ", floor=" << floor
            << ">" << std::endl;
}

int main(int argc, const char **argv[])
{
  std::string url = "mysqlx://trevor@localhost";

  std::cout << "BEFORE CONNECT TO DB" << std::endl;
  mysqlx::Session session{url.c_str()};

  std::cout << "CONNECTED TO DB" << std::endl;

  session.sql("USE plantsandthings").execute();
  std::cout << "Using plantsandthings" << std::endl;

  std::string insert_query =
      "INSERT INTO soil_moisture_sensors (name,time,ceiling,floor) VALUES ('sensor_1', '1970-01-01 00:00:01', 1.5, 2.0)";
  session.sql(insert_query.c_str()).execute();

  auto result = session.sql("SELECT * from soil_moisture_sensors").execute();
  std::list<mysqlx::Row> row_list = result.fetchAll();

  for (auto &row : row_list)
  {
    PrintRow(&row);
  }

  return 0;
}
