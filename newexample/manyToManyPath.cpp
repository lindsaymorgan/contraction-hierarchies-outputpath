/* Copyright (C) 2005, 2006, 2007, 2008
 * Robert Geisberger, Dominik Schultes, Peter Sanders,
 * Universitaet Karlsruhe (TH)
 *
 * This file is part of Contraction Hierarchies.
 *
 * Contraction Hierarchies is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * Contraction Hierarchies is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Contraction Hierarchies; see the file COPYING; if not,
 * see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "uintEdgeWeight.h"
#include "config.h"
#include "Command.h"

#include "../stats/counter.h"
#include "../stats/utils.h"
#include "../io/createGraph.h"
#include "../io/output.h"
#include "../processing/DijkstraCH.h"
#include "../processing/ConstructCH.h"
#include <google/protobuf/io/zero_copy_stream.h>

using namespace std;
using namespace google::protobuf::io;

/**
 * Performing the bucket scans is a crucial part of the many-to-many computation.
 * It can be switched off to allow time measurements of the forward search
 * without accounting for the bucket scans.
 */
const bool performBucketScans = true;
#define SCALE 1


#include "config.h"
#include "stats/utils.h"
Counter counter;
//#include "../datastr/general.h"
#include "datastr/graph/graph.h"
#include "datastr/graph/SearchGraph.h"

typedef datastr::graph::SearchGraph TransitGraph;

//#include "../stats/log.h"
#include "processing/DijkstraCH.h"
#include "many/manyToMany.h"
#include "protos/ch/many2many.pb.h"
#include "options_parser.h"

typedef datastr::graph::SearchGraph MyGraph;
typedef DijkstraCHManyToManyFW DijkstraManyToManyFW;
typedef DijkstraCHManyToManyBW DijkstraManyToManyBW;
typedef pair<NodeID, NodeID> stPair;
typedef vector<stPair> stPairs;

static void readNodes(const MyGraph *const g, const google::protobuf::RepeatedField<uint32_t>& in, vector<NodeID>* vs) {
    vs->resize(in.size());
    for (auto i=0; i < in.size(); i++) {
        // map the actual node ID to the node ID that is used internally by
        // highway-node routing
        (*vs)[i] = g->mapExtToIntNodeID(in.Get(i));
    }
}

static void readMessage(const string& filename, ::google::protobuf::Message* message) {
    int fd = open(filename.c_str(), O_RDONLY);
    message->ParseFromFileDescriptor(fd);
    close(fd);
}

static void writeMessage(const string& filename, ::google::protobuf::Message* message) {
    int fd = open(filename.c_str(), O_WRONLY);
    message->SerializeToFileDescriptor(fd);
    close(fd);
}

MyGraph *const readGraph(const string& inGraphFn) {
    ifstream inGraph(inGraphFn, ios::binary);
    if (!inGraph) {
        cerr << "Input file '" << inGraphFn << "' couldn't be opened." << endl;
        exit(-1);
    }
    return new MyGraph(inGraph);
}

class many2many_opts : public options_parser_t {
public:

    string input_fn;
    string nodes_fn;
    string matrix_fn;

    many2many_opts() : options_parser_t("i:n:o:") {}
    void parse() override {
        // input params
        set_option('i', &input_fn);

        // output params
        set_option('o', &matrix_fn);
    }
    void
    run_checks() override {
        check_not_empty(input_fn, "input file not provided (-i)");
        check_not_empty(matrix_fn, "output matrix file name not provided (-o)");
    }

    void
    usage(const char *p) override {
        cerr << "usage: " << p << " -i graph.ddsg -o matrix.pb" << endl;
    }
};


/** The main program. */
int main(int argc, char *argv[]) {

    many2many_opts opts;
    opts.parse_options(argc, argv);

    const bool writeSourceNodes = false;
    const bool writeMatrix = false;
    const bool validateResult = false;

    VERBOSE(cerr << "read graph from '" << opts.input_fn << "'" << endl);

    //new read graph
    string ddsgFile = opts.input_fn;//need edit
    ifstream in(ddsgFile.c_str());
    if (!in.is_open()) {
        cerr << "Cannot open " << ddsgFile << endl;
        exit(1);
    }

    //copy from construct.h line 248
    datastr::graph::UpdateableGraph *tGraph = importGraphListOfEdgesUpdateable(in, false, false, "");
    in.close();
    processing::DijkstraCH<datastr::graph::UpdateableGraph, NormalPQueue, 2, false> dijkstraTest(tGraph);

    //single to single test
    NodeID source, target;
    source = 234878;
    target = 618715;
    cout << "source " << source << " target " << target << endl;

    dijkstraTest.bidirSearch(source, target);
    Path path;

    clock_t start, end;
    start = clock();
    dijkstraTest.pathTo(path, target, -1);
    end = clock();
    cout << (double) (end - start) / CLOCKS_PER_SEC << endl;

    cout << source << " -> " << target << " length " << path.length() << endl;
    cout << path << endl;
    dijkstraTest.clear();
}
