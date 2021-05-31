//#include <iostream>
//#include <cstdint>
//#include <string>
//#include <fstream>
//#include "test.h"

//#include "kvstore.h"

//const bool global_test = false;

//class CorrectnessTest : public Test
//{
//private:
//    const uint64_t SIMPLE_TEST_MAX = 512;
////    const uint64_t LARGE_TEST_MAX = 1024 * 2;
//    const uint64_t LARGE_TEST_MAX = 1024 * 64;

//    void regular_test(uint64_t max)
//    {
//        //        max = 100;
//        uint64_t i;

//        // Test a single key
////        EXPECT(not_found, store.get(1));
////        store.put(1, "SE");
////        EXPECT("SE", store.get(1));
////        EXPECT(true, store.del(1));
////        EXPECT(not_found, store.get(1));
////        EXPECT(false, store.del(1));

////        phase();

//        // Test multiple key-value pairs
//        for (i = 0; i < max; ++i)
//        {
//            //            store.put(i, std::string(i*10+1, 's'));
//            store.put(i, std::string(i + 1, 's'));
//            //            EXPECT(std::string(i*10+1, 's'), store.get(i));
//            EXPECT(std::string(i + 1, 's'), store.get(i));
//        }
//        phase();

////        printf("special test  i = 0\n ");
////        EXPECT(std::string(1, 's'), store.get(0));
////        phase();

//        // Test after all insertions
//                for (i = 0; i < max; ++i)
//        //            EXPECT(std::string(i*10+1, 's'), store.get(i));
//                    EXPECT(std::string(i+1, 's'), store.get(i));
//                phase();

//                // Test deletions
//                for (i = 0; i < max; i+=2)
//                {
//                    EXPECT(true, store.del(i));

//                }
//                for (i = 0; i < max; ++i)
//        //            EXPECT((i & 1) ? std::string(i*10+1, 's') : not_found,store.get(i));
//                    EXPECT((i & 1) ? std::string(i+1, 's') : not_found,store.get(i));

//                for (i = 1; i < max; ++i)
//                    EXPECT(i & 1, store.del(i));

//        phase();

//        report();
//    }

//public:
//    CorrectnessTest(const std::string &dir, bool v = true) : Test(dir, v)
//    {
//    }

//    void start_test(void *args = NULL) override
//    {
//        if (args == NULL)
//            args = NULL;
//        std::cout << "KVStore Correctness Test" << std::endl;
//        store.reset();

//        std::cout << "[Simple Test]" << std::endl;
//        regular_test(SIMPLE_TEST_MAX);
//        store.reset();

//        std::cout << "[Large Test]" << std::endl;
//        regular_test(LARGE_TEST_MAX);
//        store.reset();
//    }
//};

//void dis_(std::string tar)
//{
//    if (tar == "")
//        std::cout << "not found" << std::endl;
//    else
//        std::cout << tar << std::endl;
//}

//struct File_t
//{
//    Header head;
//    int data = 0;
//    int ok[2];
//    File_t(){};
//    File_t(uint64_t _t, uint64_t _n, long long _max, long long _min, int _data)
//    {
//        head.time_stamp = _t;
//        head.nums = _n;
//        head.max = _max;
//        head.min = _min;
//        data = _data;
//        ok[0] = 121;
//        ok[1] = 212;
//    };
//    ~File_t(){};
//};

//void io_test()
//{
//    auto outf = []()
//    {
//        File_t f(1, 2, 3, 4, 5);
//        std::ofstream outFile("iotest.sst", std::ios::out | std::ios::binary);
//        outFile.write((char *)&f, 44);
//        outFile.close();
//    };
//    auto inf = []()
//    {
//        std::ifstream inFile("iotest.sst", std::ios::in | std::ios::binary);
//        if (!inFile)
//            dis_("error");
//        File_t ins;
//        inFile.read((char *)&ins, 44);
//        // int readBytes = inFile.gcount();
//        std::cout << ins.head.time_stamp << ',' << ins.head.nums << ',' << ins.head.max << ',' << ins.head.min << ',' << ins.data << std::endl;
//        std::cout << ins.ok[0] << ',' << ins.ok[1] << std::endl;
//        inFile.close();
//    };
//    outf();
//    inf();
//    return;
//}

//int main(int argc, char *argv[])
//{
//    if (global_test)
//    {
//        KVStore *kv_t = new KVStore("./data");
//    }
//    else
//    {
//        bool verbose = (argc == 2 && std::string(argv[1]) == "-v");
//        std::cout << std::endl;
//        std::cout << "Usage: " << argv[0] << " [-v]" << std::endl;
//        std::cout << "  -v: print extra info for failed tests [currently ";
//        std::cout << (verbose ? "ON" : "OFF") << "]" << std::endl;
//        std::cout << std::endl;
//        std::cout.flush();

//        CorrectnessTest test("./data", verbose);

//        test.start_test();
//    }
//    return 0;
//}
