//
// Created by mateus on 25/05/2021.
//

#include "ufjfmltk/Core.hpp"
#include "ufjfmltk/Visualize.hpp"
#include "ufjfmltk/Classifier.hpp"
#include "ufjfmltk/FeatureSelection.hpp"
#include <gtest/gtest.h>

class FeatselectTests: public ::testing::Test
{
protected:
    void SetUp() override{
        data = mltk::datasets::make_blobs(100, 2, 4).dataset;
        data_100 = mltk::datasets::make_blobs(100, 2, 100).dataset;
        if(!iris.load("iris.data")){
            std::cerr << "Could not load iris dataset!" << std::endl;
        }
    }
    mltk::Data<double> data, data_100, iris;
};

TEST_F(FeatselectTests, GolubTest){
    mltk::classifier::IMADual<> ima(mltk::INNER_PRODUCT);
    mltk::featselect::Golub<> golub(data, &ima, 3);

    golub.setVerbose(1);
    auto selec_data = golub.selectFeatures();

    mltk::visualize::Visualization<> vis(selec_data, false);
    vis.setTerminal("dumb");
    vis.plot3D();

    mltk::classifier::IMADual<> ima2(mltk::INNER_PRODUCT);
    mltk::featselect::Golub<> golub2(data, &ima2, 2);

    ima2.setMaxTime(10000);
    golub2.setVerbose(1);
    auto selec_data1 = golub2.selectFeatures();
    vis.setSample(selec_data1);

    vis.plot2D();
}

TEST_F(FeatselectTests, FisherTest){
    mltk::classifier::IMADual<> ima(mltk::INNER_PRODUCT);
    mltk::featselect::Fisher<> fisher(data, &ima, 3);

    fisher.setVerbose(1);
    auto selec_data = fisher.selectFeatures();
    ASSERT_EQ(selec_data.dim(), 3);
    mltk::visualize::Visualization<> vis(selec_data, false);
    vis.setTerminal("dumb");
    vis.plot3D();
    mltk::classifier::IMADual<> ima2(mltk::INNER_PRODUCT);
    mltk::featselect::Fisher<> fisher2(data, &ima2, 2);

    fisher2.setVerbose(1);
    auto selec_data1 = fisher2.selectFeatures();

    ASSERT_EQ(selec_data1.dim(), 2);
    vis.setSample(selec_data1);

    vis.plot2D();
}

TEST_F(FeatselectTests, RFETest){
    mltk::classifier::IMADual<> ima(mltk::INNER_PRODUCT);
    ima.setMaxTime(500);
    mltk::featselect::RFE<> rfe(data, &ima, 3);

    rfe.setVerbose(3);
    auto selec_data = rfe.selectFeatures();
    ASSERT_EQ(selec_data.dim(), 3);
    mltk::visualize::Visualization<> vis(selec_data, false);
    vis.setTerminal("dumb");
    vis.plot3D();
    mltk::classifier::IMADual<> ima2(mltk::INNER_PRODUCT);
    mltk::featselect::RFE<> rfe2(data, &ima2, 2);

    rfe2.setVerbose(1);
    auto selec_data1 = rfe2.selectFeatures();

    ASSERT_EQ(selec_data1.dim(), 2);
    vis.setSample(selec_data1);

    vis.plot2D();
}

TEST_F(FeatselectTests, RFECVTest){
    auto cv = mltk::validation::CrossValidation();
    cv.qtde = 1;
    cv.fold = 5;
    cv.jump = 1;
    cv.limit_error = 4;
    mltk::classifier::IMADual<> ima(mltk::INNER_PRODUCT);
    ima.setMaxTime(500);
    mltk::featselect::RFE<> rfe(data, &ima, 3, &cv);

    rfe.setVerbose(3);
    auto selec_data = rfe.selectFeatures();
    ASSERT_EQ(selec_data.dim(), 4);

    mltk::classifier::IMAp<> imap;
    ima.setMaxTime(500);
    mltk::featselect::RFE<> rfe2(data_100, &imap, 3, &cv);

    rfe2.setVerbose(3);
    auto selec_data1 = rfe2.selectFeatures();
    ASSERT_GT(selec_data1.dim(), 2);
    ASSERT_LT(selec_data1.dim(), 30);
    mltk::visualize::Visualization<> vis(selec_data1);
    vis.setTerminal("dumb");
    vis.plot3D();
}

//TEST_F(FeatselectTests, AOSCVTest){
//    auto cv = mltk::validation::CrossValidation();
//    cv.qtde = 1;
//    cv.fold = 5;
//    cv.jump = 1;
//    cv.limit_error = 4;
//    mltk::classifier::IMAp<> ima;
//    ima.setMaxTime(500);
//    mltk::featselect::AOS<> aos(iris, &ima, 2);
//
//    aos.setVerbose(3);
//    auto selec_data = aos.selectFeatures();
//    ASSERT_EQ(selec_data.dim(), 3);
//
//    mltk::visualize::Visualization<> vis(selec_data);
//   // vis.setTerminal("dumb");
//    vis.plot3D();
//
////    mltk::featselect::AOS<> aos1(data_100, &ima, 3, &cv);
////
////    aos1.setVerbose(3);
////    auto selec_data1 = aos1.selectFeatures();
////    ASSERT_EQ(selec_data1.dim(), 3);
////
////    vis.setSample(selec_data1);
////    vis.plot3D();
//}