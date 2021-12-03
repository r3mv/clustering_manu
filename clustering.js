/**
 * Distance btw cluster as minimal distance btw any
 * of the cluster members
 * clusters : array of array of elements
 * distances : array of distance objects
 * distance object [][]
 * }
 */
function minimalDistance(clusters, distances) {
    let min = Number.MAX_VALUE;
    let result = {
	"from" : -1,
	"into" : -1
    };
    for (let into = 1;  into < clusters.length; ++into) {
	for (let from = 0; from < into; ++from) {
	    if (distances[from][into] < min) {
		min = distances[from][into];
		result.from = from;
		result.into = into;
	    }
	}
    }
    return result;
}

function mergeMinLink(clusters, distanceMatrix, merge) {
    for (var elem1 of clusters[merge.from]) {
	for (var elem2 of clusters[merge.into]) {
	    distanceMatrix[elem1][elem2] = Number.MAX_VALUE;
	}
    }
    clusters[merge.into].push(...clusters.merge.from);
    clusters[merge.into].clear();
    for (let cluster of clusters) {
	if (cluster !== merge.into) {
	    let min = Number.MAX_VALUE;
	    for (let elemX of cluster) {
		for (let elemInto of clusters[merge.into]) {
		    min = min(distances[elemX][elemInto]);
		}
	    }
	    for (let elemX of cluster) {
		for (let elemInto of clusters[merge.into]) {
		    distances[elemX][elemInto] = distances[elemInto][elemX] = min;
		}
	    }
	}
    }
}


/**
 * Perform the agglomerative clustering
 * clusters : array of array of elements, initialy one element per cluster
 * distances : array of distance objects
 * linkageFunc: function returning the index of the clusters to be merged
 * mergeFunc: function actually performing the merge of clusters, updating distance matrix if needed
 *
 */
function agglomerativeHierarchicalClustering(clusters, distances, linkageFunc, mergeFunc) {
    let linkage = [];
    let numElem = clusters.length;
    while (linkage.length !== numElem - 1) {
	let merge = linkageFunction(clusters, distances);
	linkage.pushBack(merge);
	mergeFunc(clusters, distances, merge);
    }
}


function callClustering(min, max, w1, w2, w3)  {
    console.log("Calling callClustering with min=" + min
		+ " max=" + max
		+ " w1=" + w1
		+ " w2=" + w2
		+ " w3=" + w3);
    var clusters = [];
    for (let i = 0;i < flyweightDistances.numElem; ++i) {
	clusters.push([i]);
    }
    agglomerativeHierarchicalClustering(clusters,
					flyweightDistances.distances,
					minimalDistance,
					mergeMinLink);
  
}
