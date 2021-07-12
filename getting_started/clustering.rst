==========
Clustering
==========

**Clustering** is the process of grouping a set o data objects into several groups. Each group 
has points that are very similar to others in the same group, while at the same time they are 
very dissimilar to points in other groups. Similarities and dissimilarities are usually measured 
with a distance metric in clustering algorithms [HAN2011]_.

.. figure:: images/clustering/clusters.png
  :width: 450
  :name: blobs-cluster
  :align: center
  :alt: Example of blobs dataset with 5 clusters..

  Example of blobs dataset with 5 clusters.

Partitioning methods
====================

Partitioning methods are based on the construction of :math:`k` partitions of the data from a set of :math:`n` objects,
where each partition represents a cluster and :math:`k \leq n`. Usually, partitioning methods adopt *exclusive cluster separation*, i.e.,
each object must belong to exactly one group. Most partitioning methods are distance-based [HAN2011]_. 

:math:`k`-Means algorithm
^^^^^^^^^^^^^^^^^^^^^^^^^

.. [HAN2011] Han, Jiawei, Jian Pei, and Micheline Kamber. Data mining: concepts and techniques. Elsevier, 2011.