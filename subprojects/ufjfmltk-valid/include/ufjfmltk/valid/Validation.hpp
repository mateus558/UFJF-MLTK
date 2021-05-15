/*! Validation wrapper class
   \file Validation.hpp
   \author Mateus Coutinho Marim
*/

#ifndef VALIDATION__HPP
#define VALIDATION__HPP
#pragma once

#include "ufjfmltk/classifier/Classifier.hpp"
#include "ufjfmltk/classifier/PrimalClassifier.hpp"
#include "ufjfmltk/classifier/DualClassifier.hpp"
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/core/Solution.hpp"

namespace mltk{
    /**
     * \brief Namespace of methods for validation of ML algorithms.
     */
    namespace validation {
        /**
         * \brief Solution for the validation of a ML method.
         */
        struct ValidationSolution : public Solution {
            // Attributes
            /// Accuracy of the validated model.
            double accuracy = 0.0;
            /// Precision of the validated model.
            double precision = 0.0;
            /// Recall of the validated model.
            double recall = 0.0;
            /// True negative rate.
            double tnrate = 0.0;
            /// True positive rate.
            double tprate = 0.0;
            /// Number of false positives generated by the classifier in the evaluation.
            size_t falsePositive = 0;
            /// Number of false negatives generated by the classifier in the evaluation.
            size_t falseNegative = 0;
            /// Number of true negatives generated by the classifier in the evaluation.
            size_t trueNegative = 0;
            /// Number of true positives generated by the classifier in the evaluation.
            size_t truePositive = 0;
        };

        /**
         * \brief Structure to manage cross validation.
         */
        struct CrossValidation{
            size_t qtde = 0;
            int fold = 0;
            int jump = 0;
            std::vector<unsigned int> seed;

            double initial_error = 0.0;
            double actual_error = 0.0;
            double limit_error = 0.0;
        };

        /**
         * \brief A struct representing a pair with training and test data.
         */        
        template <typename T>
        struct TrainTestPair{
            /// Train data
            Data<T> train;
            /// Test data
            Data<T> test;

            TrainTestPair() = default;
            TrainTestPair(Data<T> &train, Data<T> &test){
                this->train.copy(train);
                this->test.copy(test);
            }
        };

        /**
        * \brief Compute the confusion matrix for a given trained classifier.
        * \param sample Data to train the classifier on.
        * \param classifier Classifier to be evaluated.
        * \return A matrix where the the rows are the true labels and the columns are the missclassification.
        */
        template< typename T >
        std::vector<std::vector<size_t> > generateConfusionMatrix(Data< T > &samples, Learner< T > &learner){
            auto classes = samples.classes();
            size_t size = samples.size(), i, j, idp, idy, n_classes = classes.size();
            std::vector<std::vector<size_t> > confusion_m(n_classes, std::vector<size_t>(n_classes, 0));
            double acc = 0.0;

            for(i = 0; i < size; i++){
                int pred = learner.evaluate(*(samples[i]));
                for(j = 0, idp = 0, idy = 0; j < n_classes; j++) {
                    if (classes[j] == pred) {
                        idp = j;
                    }
                    if (classes[j] == samples[i]->Y()) {
                        idy = j;
                    }
                    if (idp && idy) break;
                }
                confusion_m[idp][idy]++;
            }

//            for(i = 0; i < confusion_m.size(); i++){
//                acc += *std::max_element(confusion_m[i].begin(), confusion_m[i].end());
//            }
//            std::cout << "Purity: " << acc / size << std::endl;
            return confusion_m;
        }

       /**
        * \brief Compute the accuracy based on a confusion matrix.
        * \param conf_matrix A confusion matrix.
        * \return Accuracy based on a confusion matrix.
        */
       inline double confusionMatrixAccuracy(const std::vector<std::vector<size_t> > &conf_matrix){
            double errors = 0, total = 0;
            for(size_t i = 0; i < conf_matrix.size(); i++){
                for(size_t j = 0; j < conf_matrix[i].size(); j++){
                    if(i != j){
                        errors += conf_matrix[i][j];
                    }
                    total += conf_matrix[i][j];
                }
            }
            return (1 - errors/total)*100;
        }

        template< typename T >
        std::vector<TrainTestPair<T>> kfoldsplit(Data<T> &samples, const size_t folds, const size_t seed){
           auto data_folds = samples.splitSample(folds, seed);
           std::vector<TrainTestPair<T> > kfold_split;

           for(int i = 0; i < folds; i++){
               Data<T> train;
               for(int j = 0; j < folds; j++){
                   if(j != i){
                       train.join(data_folds[j]);
                   }
               }
               kfold_split.emplace_back(train, data_folds[i]);
           }
           return kfold_split;
       }

        template< typename T >
        std::vector<TrainTestPair<T>> kkfoldsplit(Data<T> &samples, const size_t qtde, const size_t folds, const size_t seed) {
            std::vector<TrainTestPair<T> > kkfold_split;

            kkfold_split.reserve(qtde*folds);
            for(int i = 0; i < qtde; i++){
                auto kfold_split = kfoldsplit(samples, folds, seed+i);
                kkfold_split.insert(kkfold_split.end(), kfold_split.begin(), kfold_split.end());
            }
            return kkfold_split;
        }

        /**
        * \brief Divide the samples in training and test set.
        * \param data Data to be splitted.
        * \param fold Number of folds.
        * \param seed  Seed to feed the pseudo random number generator.
        * \return A pair containing the training and test data.
        */
        template<typename T>
        TrainTestPair<T> partTrainTest(Data<T> &data, const size_t fold, const size_t seed=0) {
            std::vector<Data<T> > folds = data.splitSample(fold, seed);
            TrainTestPair<T> result;

            for(auto it = folds.begin(); it != folds.end()-1; it++){
                auto _data = *it;
                for(auto p = _data.begin(); p != _data.end(); p++){
                    auto point = *p;
                    result.train.insertPoint(point);
                }
            }
            size_t last_fold = folds.size()-1;
            for(auto it = folds[last_fold].begin(); it != folds[last_fold].end(); it++){
                auto point = *it;
                result.test.insertPoint(point);
            }

            result.train.shuffle(seed);
            result.test.shuffle(seed);

            return result;
        }

        /**
         * \brief Executes k-fold stratified cross-validation
         * \param sample Data to train the classifier on.
         * \param classifier Classifier to be evaluated.
         * \param fold Number of folds.
         * \param seed  Seed to feed the pseudo random number generator.
         * \return Classification error estimative.
         */
        template <typename T>
        double kfold (Data<T> &sample, classifier::Classifier<T> &classifier, const size_t &fold, const size_t &seed=0, const int verbose=0){
            double error = 0.0;
            std::vector<double> error_arr(fold);
            auto classes = sample.classes();
            sample.shuffle(seed);
            std::vector<Data< T > > folds = sample.splitSample(fold, seed);
            ValidationSolution solution;

            //Start cross-validation
            for(size_t fp = 0, fn = 0, tp = 0, tn = 0, j = 0; j < fold; ++j){
                auto _test_sample = folds[j];
                auto _train_sample = mltk::make_data< T >();
                for(size_t i = 0; i < fold; i++){
                    if(i != j){
                        for(auto it = folds[i].begin(); it != folds[i].end(); it++){
                            auto point = (*it);
                            _train_sample->insertPoint(point);
                        }
                    }
                }
                if(verbose){
                    std::cout << "\nCross-Validation " << j + 1 << ": \n";
                    std::cout << "Train points: " << _train_sample->size() << std::endl;
                    std::cout << "Test points: " << _test_sample.size() << std::endl;
                    std::cout << std::endl;
                }

                // Training phase
                classifier.setSamples(_train_sample);

                Solution s = classifier.getSolution();
                bool isPrimal = classifier.getFormulationString() == "Primal";
                classifier.setSeed(seed);
                if(isPrimal){
                    if(!classifier.train()){
                        if(verbose){
                            std::cerr << "Error at " << fold << "-fold: The convergency wasn't reached at the set " << j+1 << "!\n";
                        }
                    }

                    size_t i = 0;
                    for(auto it = _test_sample.begin(); it != _test_sample.end(); it++, i++){
                        auto point = (*it);
                        double _y = classifier.evaluate(*point);

                        if(point->Y() != _y){
                            if(verbose > 1)
                                std::cerr << "[" << i+1 << "x] function: " << _y << ", y: " << point->Y() << std::endl;
                            error_arr[j]++;
                            if(classes.size() == 2 && point->Y() == -1) fp++; else fn++;
                        }else{
                            if(verbose > 1)
                                std::cerr << "[" << i+1 << "] function: " << _y << ", y: " << point->Y() << std::endl;
                            if(classes.size() == 2 && point->Y() == -1) tn++; else tp++;
                        }
                    }
                }else{
                    classifier::DualClassifier< T > *dual = dynamic_cast<classifier::DualClassifier< T > *>(&classifier);
                    std::shared_ptr<Data< T > > traintest_sample(std::make_shared<Data< T > >());
                    *traintest_sample = _test_sample;
                    traintest_sample->join(*_train_sample);
                    traintest_sample->setClasses(classes);
                    dual->setSamples(traintest_sample);
                    if(!dual->train()){
                        if(verbose)
                            std::cerr << "Validation error: The convergency wasn't reached in the training set!\n";
                    }

                    size_t i = 0;
                    for(auto it = _test_sample.begin(); it != _test_sample.end(); it++, i++){
                        auto point = (*it);
                        double _y = dual->evaluate(*point);
                        if(point->Y() != _y){
                            if(verbose > 1)
                                std::cerr << "[" << i+1 << "x] function: " << _y << ", y: " << point->Y() << std::endl;
                            error_arr[j]++;
                            if(point->Y() == -1 && classes.size() == 2) fp++; else fn++;
                        }else{
                            if(verbose > 1)
                                std::cerr << "[" << i+1 << "] function: " << _y << ", y: " << point->Y() << std::endl;
                            if(point->Y() == -1 && classes.size() == 2) tn++; else tp++;
                        }
                    }

                }

                if(verbose) std::cout << "Error " << j + 1 << ": " << error_arr[j] << " -- " << ((double)error_arr[j]/(double) folds[j].size()) * 100.0f << "%\n";
                error += ((double)error_arr[j]/(double) folds[j].size()) * 100.0f;
                if(classes.size() == 2){
                    solution.accuracy += (double)(tp + tn)/(double)(tp + tn + fp + fn);
                    solution.precision += (double)tp/(double)(tp + fp);
                    solution.recall += (double)tp/(double)(tp + fn);
                    solution.tnrate += (double)tn/(double)(tn + fp);
                    solution.falseNegative += fn;
                    solution.falsePositive += fp;
                    solution.trueNegative += tn;
                    solution.truePositive += tp;
                }
            }
            if(classes.size() > 2){
                solution.accuracy += 100.0 - (((double)error)/(double)fold);
            }
            return (((double)error)/(double)fold);
        }

        /**
         * @brief Executes the validation with several executions of the k fold algorithm
         * @param sample Data to train the classifier on.
         * @param classifier Classifier to be evaluated.
         * @param fold Number of folds.
         * @param qtde Number of executions.
         * @return double Validation error.
         */
        template <typename T>
        ValidationSolution kkfold(Data<T> &samples, classifier::Classifier<T> &classifier, const size_t &qtde, const size_t &fold, const size_t &seed = 0, const int &verbose = 0){
            auto valid_pair = partTrainTest(samples, fold, seed);
            int i;
            size_t fp = 0, fn = 0, tp = 0, tn = 0, erro=0;
            double error = 0, errocross = 0, func = 0.0, margin = 0, bias;
            std::vector<double> w;
            auto classes = samples.classes();
            ValidationSolution solution;

            //sample = train_sample;

            /*cross-validation*/
            if(qtde > 0)
            {
                for(errocross = 0, i = 0; i < qtde; i++)
                {
                    if(verbose) std::cout << "\nExecucao " << i + 1 << " / " << qtde << ":\n";
                    errocross += kfold(samples, classifier, fold, seed + i, verbose);
                }
                if(verbose >= 1)std::cout << "\n\nErro " << fold << "-Fold Cross Validation: " << errocross/qtde << "%\n";
                solution.accuracy = 100.0 - errocross/qtde;
                solution.precision /= qtde*fold;
                solution.recall /= qtde*fold;
                solution.tnrate /= qtde*fold;
                solution.falseNegative /= qtde*fold;
                solution.falsePositive /= qtde*fold;
                solution.trueNegative /= qtde*fold;
                solution.truePositive /= qtde*fold;
            }

            /*start final validation*/
            if(verbose)
            {
                std::cout << "\nFinal Validation:\n";
                std::cout << "Pts de Treino: " << valid_pair.train.size() << "\n";
                std::cout << "Pts de Teste:  " << valid_pair.test.size() << "\n";
            }
            //training
            valid_pair.train.setClasses(samples.classes());
            classifier.setSamples(mltk::make_data<T>(valid_pair.train));
            classifier.setVerbose(0);

            bool isPrimal = (classifier.getFormulationString() == "Primal");
            classifier.setSeed(seed);
            if(isPrimal){
                if(!classifier.train()){
                    if(verbose)
                        std::cerr << "Validation error: The convergency wasn't reached in the training set!\n";
                }
                Solution s = classifier.getSolution();
                w = s.w;
                bias = s.bias;

                i = 0;
                for(auto it = valid_pair.test.begin(); it != valid_pair.test.end(); it++, i++){
                    auto point = (*it);
                    double _y = classifier.evaluate(*point);

                    if(point->Y() != _y){
                        if(verbose > 1)
                            std::cerr << "[" << i+1 << "x] function: " << _y << ", y: " << point->Y() << std::endl;
                        erro++;
                        if(classes.size() == 2 && point->Y() == -1) fp++; else fn++;
                    }else{
                        if(verbose > 1)
                            std::cerr << "[" << i+1 << "] function: " << _y << ", y: " << point->Y() << std::endl;
                        if(classes.size() == 2 && point->Y() == -1) tn++; else tp++;
                    }
                }
            }else{
                /*testing imadual and smo*/
                classifier::DualClassifier< T >  *dual = dynamic_cast<classifier::DualClassifier< T > *>(&classifier);
                dMatrix matrix;
                std::shared_ptr<Data< T > > traintest_sample(mltk::make_data<T>());

                *traintest_sample = valid_pair.test;
                traintest_sample->join(valid_pair.train);
                traintest_sample->setClasses(classes);
                dual->setSamples(traintest_sample);
                if(!dual->train()){
                    if(verbose)
                        std::cerr << "Validation error: The convergency wasn't reached in the training set!\n";
                }

                size_t i = 0;
                for(auto it = valid_pair.test.begin(); it != valid_pair.test.end(); it++, i++){
                    auto point = (*it);
                    double _y = dual->evaluate(*point);

                    if(point->Y() != _y){
                        if(verbose > 1)
                            std::cerr << "[" << i+1 << "x] function: " << _y << ", y: " << point->Y() << std::endl;
                        erro++;
                        if(point->Y() == -1 && classes.size() == 2) fp++; else fn++;
                    }else{
                        if(verbose > 1)
                            std::cerr << "[" << i+1 << "] function: " << _y << ", y: " << point->Y() << std::endl;
                        if(point->Y() == -1 && classes.size() == 2) tn++; else tp++;
                    }
                }
                if(verbose) std::cout.flush();
            }

            if(verbose >= 1) std::cout << "Validation Error: " << erro << " -- " << ((double)erro/(double) valid_pair.test.size()) * 100.0f << "%\n";
            error += ((double)erro/(double) valid_pair.test.size()) * 100.0f;

            return solution;
        }
    }
}
#endif
