//
// Created by Andrey Bzikadze on 1/13/21.
//

#include "gtest/gtest.h"
#include <repeat_resolution/mdbg_inc.hpp>

#include "repeat_resolution/mdbg.hpp"
#include "repeat_resolution/paths.hpp"

using namespace repeat_resolution;

logging::Logger logger;

TEST(RRPathsTest, Basic) {
  std::vector<RRPath> _path_vector;
  _path_vector.emplace_back(
      RRPath{"0", std::list<size_t>{1, 2, 3, 4, 5, 2, 6, 7, 8, 9, 10}});
  _path_vector.emplace_back(
      RRPath{"1", std::list<size_t>{11, 12, 2, 13, 14, 15, 2, 17, 18}});
  _path_vector.emplace_back(RRPath{"2", std::list<size_t>{2}});
  _path_vector.emplace_back(RRPath{"3", std::list<size_t>{2, 19}});
  _path_vector.emplace_back(RRPath{"4", std::list<size_t>{5, 2}});

  RRPaths paths = PathsBuilder::FromPathVector(_path_vector);
  const auto &path_vector = paths.GetPaths();
  const auto &ei2p = paths.GetEdge2Pos();
  const auto &eip2p = paths.GetEdgepair2Pos();
  {
    std::vector<RRPath> path_vector_ref;
    path_vector_ref.emplace_back(
        RRPath{"0", std::list<size_t>{1, 2, 3, 4, 5, 2, 6, 7, 8, 9, 10}});
    path_vector_ref.emplace_back(
        RRPath{"1", std::list<size_t>{11, 12, 2, 13, 14, 15, 2, 17, 18}});
    path_vector_ref.emplace_back(RRPath{"2", std::list<size_t>{2}});
    path_vector_ref.emplace_back(RRPath{"3", std::list<size_t>{2, 19}});
    path_vector_ref.emplace_back(RRPath{"4", std::list<size_t>{5, 2}});
    ASSERT_EQ(_path_vector, path_vector_ref);
  }

  {
    std::unordered_map<size_t, size_t> index_cnt_ref{
        {1, 1},  {2, 7},  {3, 1},  {4, 1},  {5, 2},  {6, 1},
        {7, 1},  {8, 1},  {9, 1},  {10, 1}, {11, 1}, {12, 1},
        {13, 1}, {14, 1}, {15, 1}, {17, 1}, {18, 1}, {19, 1}};
    for (const auto &pair : ei2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }
  {
    std::unordered_map<PairEdgeIndexType, size_t, PairEdgeIndexHash>
        index_cnt_ref{{{1, 2}, 1},   {{2, 3}, 1},  {{2, 19}, 1}, {{20, 2}, 1},
                      {{3, 4}, 1},   {{4, 5}, 1},  {{5, 2}, 2},  {{2, 6}, 1},
                      {{6, 7}, 1},   {{7, 8}, 1},  {{8, 9}, 1},  {{9, 10}, 1},
                      {{11, 12}, 1}, {{12, 2}, 1}, {{2, 13}, 1}, {{13, 14}, 1},
                      {{14, 15}, 1}, {{15, 2}, 1}, {{2, 17}, 1}, {{17, 18}, 1}};
    for (const auto &pair : eip2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }

  paths.remove(2);
  paths.assert_validity();
  {
    std::vector<RRPath> path_vector_ref;
    path_vector_ref.emplace_back(
        RRPath{"0", std::list<size_t>{1, 3, 4, 5, 6, 7, 8, 9, 10}});
    path_vector_ref.emplace_back(
        RRPath{"1", std::list<size_t>{11, 12, 13, 14, 15, 17, 18}});
    path_vector_ref.emplace_back(RRPath{"2", std::list<size_t>{}});
    path_vector_ref.emplace_back(RRPath{"3", std::list<size_t>{19}});
    path_vector_ref.emplace_back(RRPath{"4", std::list<size_t>{5}});
    ASSERT_EQ(path_vector, path_vector_ref);
  }
  {
    std::unordered_map<size_t, size_t> index_cnt_ref{
        {1, 1},  {3, 1},  {4, 1},  {5, 2},  {6, 1},  {7, 1},
        {8, 1},  {9, 1},  {10, 1}, {11, 1}, {12, 1}, {13, 1},
        {14, 1}, {15, 1}, {17, 1}, {18, 1}, {19, 1}};
    for (const auto &pair : ei2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }
  {
    std::unordered_map<PairEdgeIndexType, size_t, PairEdgeIndexHash>
        index_cnt_ref{{{1, 3}, 1},   {{3, 4}, 1},   {{4, 5}, 1},
                      {{5, 6}, 1},   {{6, 7}, 1},   {{7, 8}, 1},
                      {{8, 9}, 1},   {{9, 10}, 1},  {{11, 12}, 1},
                      {{12, 13}, 1}, {{13, 14}, 1}, {{14, 15}, 1},
                      {{15, 17}, 1}, {{17, 18}, 1}};
    for (const auto &pair : eip2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }
  paths.add(1, 3, 2);
  paths.assert_validity();
  {
    std::vector<RRPath> path_vector_ref;
    path_vector_ref.emplace_back(
        RRPath{"0", std::list<size_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}});
    path_vector_ref.emplace_back(
        RRPath{"1", std::list<size_t>{11, 12, 13, 14, 15, 17, 18}});
    path_vector_ref.emplace_back(RRPath{"2", std::list<size_t>{}});
    path_vector_ref.emplace_back(RRPath{"3", std::list<size_t>{19}});
    path_vector_ref.emplace_back(RRPath{"4", std::list<size_t>{5}});
    ASSERT_EQ(path_vector, path_vector_ref);
  }
  {
    std::unordered_map<size_t, size_t> index_cnt_ref{
        {1, 1},  {2, 1},  {3, 1},  {4, 1},  {5, 2},  {6, 1},  {7, 1},
        {8, 1},  {9, 1},  {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1},
        {15, 1}, {17, 1}, {18, 1}, {19, 1}, {20, 1}};
    for (const auto &pair : ei2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }
  {
    std::unordered_map<PairEdgeIndexType, size_t, PairEdgeIndexHash>
        index_cnt_ref{{{1, 2}, 1},   {{2, 3}, 1},   {{3, 4}, 1},
                      {{4, 5}, 1},   {{5, 6}, 1},   {{6, 7}, 1},
                      {{7, 8}, 1},   {{8, 9}, 1},   {{9, 10}, 1},
                      {{11, 12}, 1}, {{12, 13}, 1}, {{13, 14}, 1},
                      {{14, 15}, 1}, {{15, 17}, 1}, {{17, 18}, 1}};
    for (const auto &pair : eip2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }

  paths.merge(4, 5);
  paths.assert_validity();
  {
    std::vector<RRPath> path_vector_ref;
    path_vector_ref.emplace_back(
        RRPath{"0", std::list<size_t>{1, 2, 3, 4, 6, 7, 8, 9, 10}});
    path_vector_ref.emplace_back(
        RRPath{"1", std::list<size_t>{11, 12, 13, 14, 15, 17, 18}});
    path_vector_ref.emplace_back(RRPath{"2", std::list<size_t>{}});
    path_vector_ref.emplace_back(RRPath{"3", std::list<size_t>{19}});
    path_vector_ref.emplace_back(RRPath{"4", std::list<size_t>{4}});
    ASSERT_EQ(path_vector, path_vector_ref);
  }
  {
    std::unordered_map<size_t, size_t> index_cnt_ref{
        {1, 1},  {2, 1},  {3, 1},  {4, 2},  {6, 1},  {7, 1},
        {8, 1},  {9, 1},  {10, 1}, {11, 1}, {12, 1}, {13, 1},
        {14, 1}, {15, 1}, {17, 1}, {18, 1}, {19, 1}, {20, 1}};
    for (const auto &pair : ei2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }
  {
    std::unordered_map<PairEdgeIndexType, size_t, PairEdgeIndexHash>
        index_cnt_ref{{{1, 2}, 1},   {{2, 3}, 1},   {{3, 4}, 1},
                      {{4, 6}, 1},   {{6, 7}, 1},   {{7, 8}, 1},
                      {{8, 9}, 1},   {{9, 10}, 1},  {{11, 12}, 1},
                      {{12, 13}, 1}, {{13, 14}, 1}, {{14, 15}, 1},
                      {{15, 17}, 1}, {{17, 18}, 1}};
    for (const auto &pair : eip2p) {
      ASSERT_NE(index_cnt_ref.find(pair.first), index_cnt_ref.end());
      ASSERT_EQ(pair.second.size(), index_cnt_ref.at(pair.first));
    }
  }
}

TEST(RRPathsTest, MergeIterDereference) {
  std::vector<RRPath> _path_vector;
  _path_vector.emplace_back(RRPath{"0", std::list<size_t>{1, 2}});
  _path_vector.emplace_back(RRPath{"1", std::list<size_t>{2, 3}});

  RRPaths paths = PathsBuilder::FromPathVector(_path_vector);
  paths.merge(1, 2);
}

void CompareVertexes(
    const MultiplexDBG &graph, const std::vector<SuccinctEdgeInfo> &edge_info,
    const std::unordered_map<RRVertexType, RRVertexProperty> &isolates = {}) {
  bool IndexSetsEqual = [&graph, &edge_info, &isolates]() {
    std::unordered_set<RRVertexType> obs_vertex_set;
    for (const auto &vertex : graph) {
      obs_vertex_set.emplace(vertex);
    }

    std::unordered_set<RRVertexType> true_vertex_set;
    for (const SuccinctEdgeInfo &edge : edge_info) {
      true_vertex_set.emplace(edge.start_ind);
      true_vertex_set.emplace(edge.end_ind);
    }
    for (const auto &[isolate_index, isolate_prop] : isolates) {
      true_vertex_set.emplace(isolate_index);
    }
    return obs_vertex_set == true_vertex_set;
  }();
  VERIFY(IndexSetsEqual);

  bool PropsEquals = [&graph, &edge_info, &isolates]() {
    std::unordered_map<RRVertexType, RRVertexProperty> obs_props;
    for (const auto &vertex : graph) {
      const RRVertexProperty &vertex_prop = graph.node_prop(vertex);
      obs_props.emplace(
          vertex, RRVertexProperty(vertex_prop.Seq(), vertex_prop.IsFrozen()));
    }

    std::unordered_map<RRVertexType, RRVertexProperty> true_props;
    for (const SuccinctEdgeInfo &edge : edge_info) {
      true_props.emplace(
          edge.start_ind,
          RRVertexProperty(edge.start_prop.Seq(), edge.start_prop.IsFrozen()));
      true_props.emplace(
          edge.end_ind,
          RRVertexProperty(edge.end_prop.Seq(), edge.end_prop.IsFrozen()));
    }

    for (const auto &[isolate_index, isolate_prop] : isolates) {
      true_props.emplace(
          isolate_index,
          RRVertexProperty(isolate_prop.Seq(), isolate_prop.IsFrozen()));
    }
    return obs_props == true_props;
  }();
  VERIFY(PropsEquals);
}

void CompareEdges(const MultiplexDBG &graph,
                  const std::vector<SuccinctEdgeInfo> &edge_info) {
  int cnt = 0;
  for (const auto &vertex : graph) {
    auto [nbr_begin, nbr_end] = graph.out_neighbors(vertex);
    for (auto nbr_it = nbr_begin; nbr_it != nbr_end; ++nbr_it) {
      const RREdgeProperty &edge_prop = nbr_it->second.prop();
      const RRVertexProperty &vertex_prop = graph.node_prop(vertex);
      const RRVertexProperty &neighbor_prop = graph.node_prop(nbr_it->first);
      SuccinctEdgeInfo edge{vertex,
                            {vertex_prop.Seq(), vertex_prop.IsFrozen()},
                            nbr_it->first,
                            {neighbor_prop.Seq(), neighbor_prop.IsFrozen()},
                            edge_prop.size(),
                            edge_prop.Seq(),
                            edge_prop.IsUnique()};
      if (std::find(edge_info.begin(), edge_info.end(), edge) ==
          edge_info.end()) {
        std::cout << edge.start_ind << " " << edge.end_ind << " "
                  << List2Str(edge.seq) << "\n";
        VERIFY_MSG(false, "Found an edge that is not present among true edges");
      }
      ++cnt;
    }
  }
  VERIFY(cnt == edge_info.size());
}

using RawEdgeInfo = std::vector<std::tuple<uint64_t, uint64_t, std::string>>;
std::vector<SuccinctEdgeInfo> GetEdgeInfo(const RawEdgeInfo &raw_edge_info,
                                          size_t k, bool frozen, bool unique) {
  std::vector<SuccinctEdgeInfo> edge_info;
  for (const auto &[st, en, str] : raw_edge_info) {
    std::string prefix = str.substr(0, k);
    int64_t infix_size = ((int64_t)str.size()) - 2 * k;
    std::string infix = infix_size > 0 ? str.substr(k, infix_size) : "";
    std::string suffix = str.substr(str.size() - k);
    edge_info.push_back({st,
                         {Str2List(prefix), frozen},
                         en,
                         {Str2List(suffix), frozen},
                         infix_size,
                         Str2List(infix),
                         unique});
  }
  return edge_info;
}

TEST(DB1, Basic) {
  const size_t k = 2;

  std::vector<SuccinctEdgeInfo> edge_info = [k]() {
    const bool frozen = false;
    const bool unique = false;
    std::vector<std::tuple<RRVertexType, RRVertexType, std::string>>
        raw_edge_info{{0, 2, "CCT"},  // 0
                      {1, 2, "GACT"}, // 1
                      {2, 3, "CTAG"}, // 2
                      {3, 4, "AGTT"}, // 3
                      {3, 5, "AGC"},  // 4
                      {2, 4, "CTT"}}; // 5
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k, frozen, unique);
    return edge_info;
  }();

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2, 3}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{1, 5}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  {
    CompareVertexes(mdbg, edge_info);
    CompareEdges(mdbg, edge_info);
  }
}

TEST(DBSingleEdge1, Basic) {
  const size_t k = 2;

  std::vector<SuccinctEdgeInfo> edge_info = [k]() {
    const bool frozen = false;
    const bool unique = false;
    std::vector<std::tuple<RRVertexType, RRVertexType, std::string>>
        raw_edge_info{{0, 1, "ACGTTGCA"}}; // 0
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k, frozen, unique);
    return edge_info;
  }();

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 1, "ACGTTGCA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::unordered_map<RRVertexType, RRVertexProperty> isolates;

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

TEST(DBSingleEdge2, Basic) {
  const size_t k = 2;

  std::vector<SuccinctEdgeInfo> edge_info = [k]() {
    const bool frozen = false;
    const bool unique = false;
    std::vector<std::tuple<RRVertexType, RRVertexType, std::string>>
        raw_edge_info{{0, 1, "ACGCA"}}; // 0
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k, frozen, unique);
    return edge_info;
  }();

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 1, "ACGCA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::unordered_map<RRVertexType, RRVertexProperty> isolates;

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

TEST(DBSingleEdge3, Basic) {
  const size_t k = 2;

  std::vector<SuccinctEdgeInfo> edge_info = [k]() {
    const bool frozen = false;
    const bool unique = false;
    std::vector<std::tuple<RRVertexType, RRVertexType, std::string>>
        raw_edge_info{{0, 1, "ACGTGCA"}}; // 0
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k, frozen, unique);
    return edge_info;
  }();

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);
  int N = 5;

  MultiplexDBGIncreaser k_increaser{k, k + N, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + N, false, false);

    std::unordered_map<RRVertexType, RRVertexProperty> isolates;
    isolates.emplace(0, RRVertexProperty(Str2List("ACGTGCA"), true));

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

TEST(DBStVertex, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "AAAAA"}, {0, 2, "AAACA"}, {0, 3, "AAA"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {4, 1, "AAAAA"}, {5, 2, "AAACA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    std::unordered_map<RRVertexType, RRVertexProperty> isolates;
    isolates.emplace(6, RRVertexProperty(Str2List("AAA"), true));

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

TEST(DBEvVertex, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 3, "AAAAA"}, {1, 3, "AACAA"}, {2, 3, "AAA"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  //  for (const RRVertexType &vertex : mdbg) {
  //    std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //              << mdbg.count_out_neighbors(vertex) << " "
  //              << mdbg.node_prop(vertex).size << "\n";
  //  }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 4, "AAAAA"}, {1, 5, "AACAA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    std::unordered_map<RRVertexType, RRVertexProperty> isolates;
    isolates.emplace(2, RRVertexProperty(Str2List("AAA"), true));

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

// graph 1-in >1-out
TEST(DB1inVertex, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "AACAG"}, {1, 2, "AGACC"}, {1, 3, "AGATT"}, {1, 4, "AGAGG"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  //  for (const RRVertexType &vertex : mdbg) {
  //    std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //              << mdbg.count_out_neighbors(vertex) << " "
  //              << mdbg.node_prop(vertex).size << "\n";
  //  }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 1, "AACAG"}, {1, 2, "CAGACC"}, {1, 3, "CAGATT"}, {1, 4, "CAGAGG"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::unordered_map<RRVertexType, RRVertexProperty> isolates{};

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

// graph 1-in >1-out with 1-in transforming into a vertex
TEST(DB1inVertex, WithShortEdge) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "CAG"}, {1, 2, "AGACC"}, {1, 3, "AGATT"}, {1, 4, "AGAGG"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 2, "CAGACC"}, {0, 3, "CAGATT"}, {0, 4, "CAGAGG"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph >1-in 1-out
TEST(DB1outVertex, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 3, "CCAGA"}, {1, 3, "TTAGA"}, {2, 3, "GGAGA"}, {3, 4, "GAAAA"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  //  for (const RRVertexType &vertex : mdbg) {
  //    std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //              << mdbg.count_out_neighbors(vertex) << " "
  //              << mdbg.node_prop(vertex).size << "\n";
  //  }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 3, "CCAGAA"}, {1, 3, "TTAGAA"}, {2, 3, "GGAGAA"}, {3, 4, "GAAAA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::vector<RRVertexType> isolates{};

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph >1-in 1-out with 1-in transforming into a vertex
TEST(DB1outVertex, WithShortEdge) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 3, "CCAGA"}, {1, 3, "TTAGA"}, {2, 3, "GGAGA"}, {3, 4, "GAA"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  //  for (const RRVertexType &vertex : mdbg) {
  //    std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //              << mdbg.count_out_neighbors(vertex) << " "
  //              << mdbg.node_prop(vertex).size << "\n";
  //  }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 3, "CCAGAA"}, {1, 3, "TTAGAA"}, {2, 3, "GGAGAA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::vector<RRVertexType> isolates{};

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (2in-2out)
TEST(DBComplexVertex, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"}, {1, 2, "GGAAA"}, {2, 3, "AATGC"}, {2, 4, "AATT"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{1, 3}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 3, "ACAAATGC"}, {1, 4, "GGAAATT"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (loop)
TEST(DBComplexVertexLoop1, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"}, {2, 2, "AAGAA"}, {2, 3, "AATGC"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{1, 2}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 3, "ACAAAGAATGC"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::vector<RRVertexType> isolates{};

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (loop + another traversal)
TEST(DBComplexVertexLoop2, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"},
      {2, 2, "AAGAA"},
      {2, 3, "AATGC"},
      {4, 2, "GGAA"},
      {2, 5, "AATG"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{1, 2}});
    _path_vector.emplace_back(RRPath{"2", std::list<size_t>{3, 4}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 3, "ACAAAGAATGC"}, {4, 5, "GGAATG"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (two loops)
TEST(DBComplexVertexLoop3, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"}, {2, 2, "AAGAA"}, {2, 3, "AATGC"},
      {4, 2, "GGAA"},  {2, 2, "AAA"},   {2, 5, "AATG"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1, 2}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{3, 4, 5}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 3, "ACAAAGAATGC"}, {4, 5, "GGAAATG"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    const std::vector<RRVertexType> isolates{};

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (multiple loops)
TEST(DBComplexVertexLoop4, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "ACAAA"}, {1, 1, "AAGAA"}, {1, 1, "AACAA"},
      {1, 1, "AATAA"}, {1, 1, "AAAAA"}, {1, 2, "AATGC"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1, 2, 3, 4, 5}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 2, "ACAAAGAACAATAAAAATGC"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (multiple loops, several traversals)
TEST(DBComplexVertexLoop5, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "ACAAA"},  // 0
      {1, 1, "AAGAA"},  // 1
      {1, 1, "AACAA"},  // 2
      {1, 1, "AATAA"},  // 3
      {1, 1, "AAAAA"},  // 4
      {1, 2, "AATGC"},  // 5
      {3, 1, "ACAAA"},  // 6
      {1, 4, "AATGC"},  // 7
      {5, 1, "ACAAA"},  // 8
      {1, 6, "AATGC"}}; // 9
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1, 2, 5}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{6, 3, 4, 7}});
    _path_vector.emplace_back(RRPath{"2", std::list<size_t>{8, 9}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 2, "ACAAAGAACAATGC"}, {3, 4, "ACAAATAAAAATGC"}, {5, 6, "ACAAATGC"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with two buldges and loops inside
TEST(DBBuldges1, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "ACAAA"},  // 0
      {1, 1, "AAGAA"},  // 1
      {1, 2, "AACGC"},  // 2
      {0, 1, "ACTAA"},  // 3
      {1, 1, "AAAAA"},  // 4
      {1, 2, "AATGC"},  // 5
      {0, 1, "ACAAA"},  // 6
      {1, 2, "AATGC"}}; // 7
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1, 2}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{3, 4, 5}});
    _path_vector.emplace_back(RRPath{"2", std::list<size_t>{6, 7}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {6, 3, "ACAAAGAACGC"}, {7, 4, "ACTAAAAATGC"}, {8, 5, "ACAAATGC"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with complex vertex and 4 connections
TEST(DBComplexVertexConn4, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"}, {1, 2, "GGAAA"}, {2, 3, "AATGC"}, {2, 4, "AATT"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{0, 3}});
    _path_vector.emplace_back(RRPath{"2", std::list<size_t>{1, 2}});
    _path_vector.emplace_back(RRPath{"3", std::list<size_t>{1, 3}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 5, "ACAAA"}, {5, 7, "AAAT"}, {7, 3, "AATGC"}, {5, 8, "AAAT"},
        {1, 6, "GGAAA"}, {6, 7, "AAAT"}, {6, 8, "AAAT"},  {8, 4, "AATT"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with complex vertex and 3 connections
TEST(DBComplexVertexConn3, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"}, {1, 2, "GGAAA"}, {2, 3, "AATGC"}, {2, 4, "AATT"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2}});
    _path_vector.emplace_back(RRPath{"1", std::list<size_t>{0, 3}});
    // _path_vector.emplace_back(RRPath{"2", std::list<size_t>{1, 2}});
    _path_vector.emplace_back(RRPath{"3", std::list<size_t>{1, 3}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 5, "ACAAA"},
        {5, 3, "AAATGC"},
        {5, 8, "AAAT"},
        {1, 8, "GGAAAT"},
        {8, 4, "AATT"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with complex vertex and 3 connections
TEST(DBComplexVertexConn3_2, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 2, "ACAAA"}, {1, 2, "GGAAA"}, {2, 3, "AATGC"}, {2, 4, "AATT"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2}});
    // _path_vector.emplace_back(RRPath{"1", std::list<size_t>{0, 3}});
    _path_vector.emplace_back(RRPath{"2", std::list<size_t>{1, 2}});
    _path_vector.emplace_back(RRPath{"3", std::list<size_t>{1, 3}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 7, "ACAAAT"},
        {7, 3, "AATGC"},
        {6, 7, "AAAT"},
        {1, 6, "GGAAA"},
        {6, 4, "AAATT"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (loop)
TEST(DBComplexVertexLoop6, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "ACAAA"}, {1, 1, "AAGAA"}, {1, 2, "AATGC"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 1, 1, 2}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);
  int N = 4;

  MultiplexDBGIncreaser k_increaser{k, k + N, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 2, "ACAAAGAAGAATGC"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + N, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (loop)
TEST(DBComplexVertexLoop7, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      // {0, 1, "ACAAA"},
      {1, 1, "AAGAA"}};
  // {1, 2, "AATGC"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    // _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2}});
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 0}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {1, 1, "AAGAA"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k, true, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a complex vertex (loop)
TEST(DBComplexVertexLoop8, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "ACAAA"}, {1, 1, "AAGAA"}, {1, 2, "AATGC"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{0, 2}});
    _path_vector.emplace_back(RRPath{"0", std::list<size_t>{1, 1}});

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
        {0, 2, "ACAAATGC"}, {6, 6, "AAGAAG"}};
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);
    edge_info[1].start_prop.freeze();
    edge_info[1].end_prop.freeze();

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
  }
}

// graph with a single edge that will be isolated
TEST(DBIsolate, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info{
      {0, 1, "ACA"}};
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  // for (const RRVertexType &vertex : mdbg) {
  //   std::cout << vertex << " " << mdbg.count_in_neighbors(vertex) << " "
  //             << mdbg.count_out_neighbors(vertex) << " "
  //             << mdbg.node_prop(vertex).size << "\n";
  // }
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info;
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    std::unordered_map<RRVertexType, RRVertexProperty> isolates;
    isolates.emplace(0, RRVertexProperty(Str2List("ACA"), true));

    CompareVertexes(mdbg, edge_info, isolates);
    CompareEdges(mdbg, edge_info);
  }
}

TEST(DBEmptyGraph, Basic) {
  const size_t k = 2;

  std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info;
  std::vector<SuccinctEdgeInfo> edge_info =
      GetEdgeInfo(raw_edge_info, k, false, false);

  RRPaths paths = []() {
    std::vector<RRPath> _path_vector;

    return PathsBuilder::FromPathVector(_path_vector);
  }();

  MultiplexDBG mdbg(edge_info, k, &paths);

  MultiplexDBGIncreaser k_increaser{k, k + 1, logger, true};
  k_increaser.IncreaseUntilSaturation(mdbg);
  {
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> raw_edge_info;
    std::vector<SuccinctEdgeInfo> edge_info =
        GetEdgeInfo(raw_edge_info, k + 1, false, false);

    CompareVertexes(mdbg, edge_info, {});
    CompareEdges(mdbg, edge_info);
    ASSERT_TRUE(mdbg.IsFrozen());
  }
}

TEST(RC, Basic) {
  ASSERT_EQ(GetRC(Str2List("AATTCCGG")), Str2List("CCGGAATT"));
  ASSERT_EQ(GetRC({}), Str2List({}));

  ASSERT_TRUE(IsCanonical(Str2List("AATTCCGG")));
  ASSERT_FALSE(IsCanonical(Str2List("CCGGAATT")));
  ASSERT_TRUE(IsCanonical(Str2List("ACGT")));
}