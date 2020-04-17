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
  size_t index = value.get_uint();

  value = reinterpret_cast<V &>(row->get(1));
  const std::u16string &wide_name = value.get_ustring();
  const std::string &name = std::string(reinterpret_cast<const char *>(wide_name.c_str()), wide_name.size() * 2);

  value = reinterpret_cast<V &>(row->get(2));
  const std::u16string &wide_time = value.get_ustring();
  const std::string &time = std::string(reinterpret_cast<const char *>(wide_time.c_str()), wide_time.size() * 2);

  value = reinterpret_cast<V &>(row->get(2));
  const std::u16string &wide_location = value.get_ustring();
  const std::string &location = std::string(reinterpret_cast<const char *>(wide_location.c_str()), wide_location.size() * 2);

  std::cout << "Row: <index=" << index
            << ", name=" << name
            << ", time=" << time
            << ", location=" << location
            << ">" << std::endl;
}

int main(int argc, const char **argv)
{
  std::string url = "mysqlx://trevor@localhost";

  std::cout << "BEFORE CONNECT TO DB" << std::endl;
  mysqlx::Session session{url.c_str()};

  std::cout << "CONNECTED TO DB" << std::endl;

  session.sql("USE plantsandthings").execute();
  std::cout << "Using plantsandthings" << std::endl;

  std::string insert_query =
      "INSERT INTO rpis (name,time,location) VALUES ('rpi_1', '1970-01-01 00:00:01', 'test location')";
  session.sql(insert_query.c_str()).execute();

  auto result = session.sql("SELECT * from rpis").execute();
  std::list<mysqlx::Row> row_list = result.fetchAll();

  for (auto &row : row_list)
  {
    PrintRow(&row);
  }

  return 0;
}
