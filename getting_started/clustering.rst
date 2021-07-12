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

The :math:`k`-Means algorithm
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :math:`k`-means algorithm works by first assigning as centroids of the clusters :math:`k` random points from the dataset, representing the mean of the clusters. For each 
remaining objects, each of them is assigned to the cluster were it is most similar based on the euclidean distance and the cluster mean. Then, it iteratively improves the within-cluster
variation. At each iteration it computes the new clusters means using the objects assigned in the previous iteration and only stops when the assignment is stable, that is, the clusters
formed in the current round are the same as those formed in the previous round [HAN2011]_.

.. [HAN2011] Han, Jiawei, Jian Pei, and Micheline Kamber. Data mining: concepts and techniques. Elsevier, 2011.