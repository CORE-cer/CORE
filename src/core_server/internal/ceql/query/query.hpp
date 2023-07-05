#pragma once
#include "consume_by.hpp"
#include "from.hpp"
#include "partition_by.hpp"
#include "select.hpp"
#include "where.hpp"
#include "within.hpp"

namespace CORE::Internal::CEQL {
struct Query {
  Select select;
  From from;
  Where where;
  PartitionBy partition_by;
  Within within;
  ConsumeBy consume_by;

  Query(Select&& select,
        From&& from,
        Where&& where,
        PartitionBy&& partition_by,
        Within&& within,
        ConsumeBy&& consume_by)
      : select(std::move(select)),
        from(std::move(from)),
        where(std::move(where)),
        partition_by(std::move(partition_by)),
        within(std::move(within)),
        consume_by(std::move(consume_by)) {}
};
}  // namespace CORE::Internal::CEQL
