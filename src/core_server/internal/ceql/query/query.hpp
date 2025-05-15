#pragma once
#include <string>
#include <utility>

#include "consume_by.hpp"
#include "from.hpp"
#include "limit.hpp"
#include "partition_by.hpp"
#include "select.hpp"
#include "where.hpp"
#include "within.hpp"

namespace CORE::Internal::CEQL {
struct Query {
  Select select;  // TODO Projection
  From from;      // TODO: Routing.
  Where where;
  PartitionBy partition_by;  // TODO
  Within within;
  ConsumeBy consume_by;
  Limit limit;

  Query(Select&& select,
        From&& from,
        Where&& where,
        PartitionBy&& partition_by,
        Within&& within,
        ConsumeBy&& consume_by,
        Limit&& limit)
      : select(std::move(select)),
        from(std::move(from)),
        where(std::move(where)),
        partition_by(std::move(partition_by)),
        within(std::move(within)),
        consume_by(std::move(consume_by)),
        limit(std::move(limit)) {}

  Query(const Query& other)
      : select(other.select.clone()),
        from(other.from),
        where(other.where.clone()),
        partition_by(other.partition_by),
        within(other.within),
        consume_by(other.consume_by),
        limit(other.limit) {}

  Query(Query&& other) noexcept = default;

  Query& operator=(Query&& other) noexcept = default;

  ~Query() = default;

  std::string to_string() const {
    std::string out = select.to_string() + "\n" + from.to_string() + "\n"
                      + where.to_string() + "\n" + partition_by.to_string() + "\n"
                      + within.to_string() + "\n" + consume_by.to_string();
    return out;
  }
};
}  // namespace CORE::Internal::CEQL
