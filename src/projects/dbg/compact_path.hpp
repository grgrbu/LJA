#pragma once
#include "sparse_dbg.hpp"
#include "graph_algorithms.hpp"
#include "graph_stats.hpp"
#include <parallel/algorithm>
#include <utility>
namespace dbg {

    class CompactPath {
    private:
        Vertex *_start;
        size_t _first_skip;
        size_t _last_skip;
    public:
        Sequence _edges;
        CompactPath() : _start(nullptr), _first_skip(0), _last_skip(0) {
        }

        CompactPath(Vertex &start, const Sequence &edges, size_t first_skip = 0, size_t last_skip = 0) :
                _start(&start), _edges(edges), _first_skip(first_skip), _last_skip(last_skip) {
        }

        explicit CompactPath(const Path &path, size_t first_skip = 0, size_t last_skip = 0) :
                _start(&path.getVertex(0)), _first_skip(first_skip), _last_skip(last_skip) {
            std::vector<char> edges;
            for (const auto &edge: path) {
                edges.push_back(edge->seq[0]);
            }
            _edges = Sequence(edges);
        }

        explicit CompactPath(const GraphAlignment &path) :
                _start(&path.getVertex(0)), _first_skip(path.leftSkip()), _last_skip(path.rightSkip()) {
            std::vector<char> edges;
            for (const auto &seg: path) {
                edges.push_back(seg.contig().seq[0]);
            }
            _edges = Sequence(edges);
        }

        explicit CompactPath(const GraphAlignment &path, size_t left, size_t right) :
                _start(&path.getVertex(left)), _first_skip(path[left].left), _last_skip(path[right - 1].RC().left) {
            std::vector<char> edges;
            for (size_t i = left; i < right; i++) {
                edges.push_back(path[i].contig().seq[0]);
            }
            _edges = Sequence(edges);
        }

        bool valid() const {
            return _start != nullptr;
        }

        GraphAlignment getAlignment() const {
            if (!valid())
                return {};
            std::vector<Segment<Edge>> path;
            Vertex *cur = _start;
            for (size_t i = 0; i < _edges.size(); i++) {
                VERIFY(cur->hasOutgoing(_edges[i]));
                Edge &edge = cur->getOutgoing(_edges[i]);
                path.emplace_back(edge, 0, edge.size());
                cur = edge.end();
            }
            if (_first_skip > 0)
                path.front().left += _first_skip;
            if (_last_skip > 0)
                path.back().right -= _last_skip;
            return {_start, std::move(path)};
        }

        Path getPath() const {
            std::vector<Edge *> path;
            Vertex *cur = _start;
            for(size_t i = 0; i < _edges.size(); i++) {
                Edge &edge = cur->getOutgoing(_edges[i]);
                path.emplace_back(&edge);
                cur = edge.end();
            }
            return {*_start, std::move(path)};
        }

        std::vector<Edge *>getPathVector() const {
            std::vector<Edge *> path;
            Vertex *cur = _start;
            for(size_t i = 0; i < _edges.size(); i++) {
                Edge &edge = cur->getOutgoing(_edges[i]);
                path.emplace_back(&edge);
                cur = edge.end();
            }
            return path;
        }

        CompactPath RC() const {
            if (!valid())
                return {};
            return CompactPath(getAlignment().RC());
        }

//    Vertex &start() {
//        return *_start;
//    }

        Vertex &start() const {
            return *_start;
        }

        const Sequence &cpath() const {
            return _edges;
        }

        size_t leftSkip() const {
            return _first_skip;
        }

        size_t rightSkip() const {
            return _last_skip;
        }

        size_t size() const {
            return _edges.size();
        }

        unsigned char operator[](size_t ind) const {
            return _edges[ind];
        }

        static CompactPath Load(std::istream &os, SparseDBG &dbg) {
            hashing::htype hash;
            bool canonical;
            size_t left = 0;
            size_t right = 0;
            std::string path;
            os >> hash >> canonical >> path >> left >> right;
            path = path.substr(2);
            if(hash == 0 && path.size() == 0) {
                return {};
            }
            return {dbg.getVertex(hash, canonical), Sequence(path), left, right};
        }
    };
}
inline std::ostream& operator<<(std::ostream  &os, const dbg::CompactPath &cpath) {
    if(cpath.valid()) {

        for(size_t i = 0; i < cpath._edges.size(); i++) {
//            dbg::Edge edge = cpath[i];
//            os << edge.id << " ";
            os << cpath[i] << " ";
        }
        os << "\n";
        return os << cpath.start().hash() << " " << cpath.start().isCanonical() << " P:" << cpath.cpath() << " "
                  << cpath.leftSkip() << " " << cpath.rightSkip();
    } else {
        return os << "0 0 P: 0 0";
    }
}