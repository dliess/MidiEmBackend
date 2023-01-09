#include <gtest/gtest.h>
#include "Identifiable.h"

#include <unordered_map>

TEST(IdentifiableTest, Map) {
    std::unordered_map<util::Identifiable::UUID, int, util::IdentifiableHash, std::equal_to<>> map;
    util::Identifiable::UUID uuid1 {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    util::Identifiable::UUID uuid2 {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    util::Identifiable::UUIDView uuidView1 = uuid1;
    util::Identifiable::UUIDView uuidView2 = uuid2;
    map[uuid1] = 5;
    map[uuid2] = 8;

    // TODO: from gcc11 this should work without the deepcopy 
    const auto it = map.find(util::deepCopy(uuidView1));
}