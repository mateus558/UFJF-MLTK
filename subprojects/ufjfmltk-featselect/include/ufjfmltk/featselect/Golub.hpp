//
// Created by Mateus Coutinho Mari on 7/27/2018.
//

#ifndef UFJF_MLTK_GOLUB_H
#define UFJF_MLTK_GOLUB_H
#pragma once
#include "FeatureSelection.hpp"

namespace mltk{
    namespace featselect {
        template<typename T>
        class Golub : public FeatureSelection<T> {
        private:
            int number;

            struct golub_select_score {
                int fname;
                double score;
            };

            static int golub_select_compare_score_greater(const golub_select_score &a, const golub_select_score &b);

        public:
            explicit Golub(std::shared_ptr<Data<T> > samples = nullptr, classifier::Classifier<T> *classifier = nullptr,
                           int number = 0);

            std::shared_ptr<Data<T> > selectFeatures() override;
        };
    }
}

#endif //UFJF_MLTK_GOLUB_H
