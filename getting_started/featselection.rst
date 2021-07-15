=================
Feature Selection
=================

**Feature selection** is the process of selecting a subset from the original feature set,
following some criterion, without changing the results [VILLELA2011]_.


Filter methods
==============

Filter selection methods introduces a serate step before the learning algorithm application. This preprocessing
step consider some general features from samples to select some attributes and remove others. It acts by utilizing some
criterion to "filter" the irrelevant features [VILLELA2011]_.

Golub and Fisher
^^^^^^^^^^^^^^^^

These methods are based on the different of the magnitude of the expression levels of each feature.
A feature can be said differentially expressed in two distinct classes if the difference between the 
weighted average of these classes divided by the sum of the standard deviation is high. Defining :math:`\mu_1`
and :math:`\mu_2` as both classes average, and :math:`\sigma_1` and :math:`\sigma_2` as its standard deviations, respectively.
The scores Golub :math:`G` and Fisher :math:`F` can be defined as following:

.. math::

    G = \frac{| \mu_1 - \mu_2 |}{(\sigma_1 + \sigma_2)}

.. math::

    F = \frac{( \mu_1 - \mu_2 )^2}{(\sigma_1 + \sigma_2)}


.. code-block:: cpp

    #include <ufjfmltk/ufjfmltk.hpp>

    namespace classifier = mltk::classifier;
    namespace feat = mltk::featselect;

    int main(){
        auto data = mltk::datasets::make_blobs(100, 2, 30).dataset;
        classifier::IMAp<double> ima;
        feat::Golub<double> g(data, &ima, 3);
        feat::Fisher<double> f(data, &ima, 3);

        std::cout << "Initial dims: " << data.dim() << std::endl;
        mltk::Data gresult = g.selectFeatures();
        std::cout << "Final dims: " << gresult.dim() << std::endl;

        std::cout << "\nInitial dims: " << data.dim() << std::endl;
        mltk::Data fresult = f.selectFeatures();
        std::cout << "Final dims: " << fresult.dim() << std::endl;
    }

Embedded methods
================

This strategy is based on the fact that some induction algorithms can perform their 
own feature selection. These methods use the induction algorithm to estimate the value 
of the selected feature subset during the training phase and they generaly are specific
for a given classification algorithm [VILLELA2011]_.  

Wrapper methods
===============

The wrapper approach generates several features subsets as candidates, executes the induction 
algorithm individually in each subset and uses the classifier precision to evaluate the subset in
question. The process is repeated until it reaches a stop criterion. The general idea is that the 
feature selection algorithm exists as a wrapper around the inductor that is responsible to conduct 
the search by a good feature subset [VILLELA2011]_. 

Recursive Feature Elimination (RFE)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This method works by removing a fixed number of features at a time. The features selected for removal
at each step are those that have the smallest rankings computed using a classifier trained after each removal.
The elimination of only one feature at a time generates a classifier with less errors. For computational reasons,
its prefered to remove several features at a time, at the expense of the classifier degradation [GUYON2002]_.

.. code-block:: cpp

    #include <ufjfmltk/ufjfmltk.hpp>

    namespace classifier = mltk::classifier;
    namespace feat = mltk::featselect;

    int main(){
        auto data = mltk::datasets::make_blobs(100, 2, 30).dataset;
        classifier::IMAp<double> ima;
        feat::RFE<double> rfe(data, &ima, 3);

        std::cout << "Initial dims: " << data.dim() << std::endl;
        mltk::Data rfe_result = rfe.selectFeatures();
        std::cout << "Final dims: " << rfe_result.dim() << std::endl;

    }


Admissible Ordered Search (AOS)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This wrapper method uses as predictor a large margin classifier, such as IMA [VILLELA2016]_. For each dimension, it finds the 
classifier with the largest margin [VILLELA2015]_. *TODO*

.. code-block:: cpp

    #include <ufjfmltk/ufjfmltk.hpp>

    namespace classifier = mltk::classifier;
    namespace feat = mltk::featselect;

    int main(){
        auto data = mltk::datasets::make_blobs(100, 2, 30).dataset;
        classifier::IMAp<double> ima;
        feat::AOS<double> aos(data, &ima, 3);

        std::cout << "Initial dims: " << data.dim() << std::endl;
        mltk::Data aos_result = aos.selectFeatures();
        std::cout << "Final dims: " << aos_result.dim() << std::endl;

    }

.. [VILLELA2011] Villela, Saulo Moraes, et al. "Seleção de Características utilizando Busca Ordenada e um Classificador de Larga Margem." (2011).
.. [GUYON2002] Guyon, Isabelle, et al. "Gene selection for cancer classification using support vector machines." Machine learning 46.1 (2002): 389-422.
.. [VILLELA2015] Villela, Saulo Moraes, Saul de Castro Leite, and Raul Fonseca Neto. "Feature selection from microarray data via an ordered search with projected margin." Twenty-Fourth International Joint Conference on Artificial Intelligence. 2015.
.. [VILLELA2016] Villela, Saulo Moraes, Saul de Castro Leite, and Raul Fonseca Neto. "Incremental p-margin algorithm for classification with arbitrary norm." Pattern Recognition 55 (2016): 261-272.