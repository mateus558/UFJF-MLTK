//
// Created by Mateus Coutinho Marim on 7/28/2018.
//

/*****************************************************
 * SMO classifier lib                                *
 *****************************************************/

#ifndef UFJF_MLTK_SMO_H
#define UFJF_MLTK_SMO_H
#pragma once

#include "DualClassifier.hpp"
#include <list>

namespace mltk{
        namespace classifier {
            /*double linked list of integers*/
            struct int_dll {
                int index = -1;
                int_dll *prev = nullptr;
                int_dll *next = nullptr;

                int_dll();

                static int_dll *remove(int_dll **node);

                static int_dll *append(int_dll *list);

                static void free(int_dll **head);
            };

            typedef struct int_dll int_dll;

            template<typename T>
            class SMO : public DualClassifier<T> {
            private:
                struct smo_learning_data {
                    double error;
                    char done;
                    int_dll *sv = nullptr;

                    smo_learning_data() {
                        error = 0.0;
                        done = 0;
                        sv = nullptr;
                    }
                };
                double param = 0;
                const int C = 9999; //0.05
                const double TOL = 0.0001;
                std::vector<smo_learning_data> l_data;
                int_dll *head = nullptr;

                bool examine_example(int i1);

                bool max_errors(int i1, double e1);

                bool iterate_non_bound(int i1);

                bool iterate_all_set(int i1);

                int take_step(int i1, int i2);

                double function(int index);

                bool training_routine();

                void test_learning();

                int train_matrix(Kernel *matrix);

            public:
                SMO() = default;
                explicit SMO(const Data<T>& samples, KernelType kernel_type = KernelType::INNER_PRODUCT,
                             double param = 0, int verbose = 0);

                bool train() override;

                ~SMO();
            };
        }
}
#endif //UFJF_MLTK_SMO_H
