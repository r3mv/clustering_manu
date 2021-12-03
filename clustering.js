var linkage = undefined;

/**
 * Return the index of the clusters to be merged,
 * that is clusters having the minimal distance in the distance matrix
 * distances: [][] matrix of distances between clusters
 */
function findClustersToMerge(distances) {
    let min = Number.MAX_VALUE;
    let result = {
	"from" : -1,
	"into" : -1
    };

    // note: merge cluster with bigger index in cluster with smaller index
    for (let from = 1;  from < distances.length; ++from) {
	for (let into = 0; into < from; ++into) {
	    if (distances[from][into] < min) {
		min = distances[from][into];
		result.from = from;
		result.into = into;
	    }
	}
    }
    result.distance = min;
    return result;
}

function mergeMinLink(clusters, distances, merge) {
    for (let i = 0;i < clusters[merge.from].length;++i) {
	for (let j = 0; j < clusters[merge.into].length; ++j) {
	    let elem1 = clusters[merge.from][i];
	    let elem2 = clusters[merge.into][j];
	    distances[elem1][elem2] = distances[elem2][elem1] = Number.MAX_VALUE;
	}
    }
    clusters[merge.into].push(...clusters[merge.from]);
    clusters[merge.from] = [];
    for (let x = 0;x < clusters.length; ++x) {
	if (x !== merge.into) {
	    // dist btw cluster x and y is the min dist of any elements in cluster x and y
	    let min = Number.MAX_VALUE;
	    for (let i = 0;i < clusters[x].length; ++i) {
		let outerElem = clusters[x][i];
		for (let j = 0;j < clusters[merge.into].length; ++j) {
		    let innerElem = clusters[merge.into][j];
		    if (distances[outerElem][innerElem] < min) {
			min = distances[outerElem][innerElem];
		    }
		}
	    }
	    for (let i = 0;i < clusters[x].length; ++i) {
		for (let j = 0; j < clusters[merge.into].length; ++j) {
		    let outerElem = clusters[x][i];
		    let innerElem = clusters[merge.into][j];
		    distances[outerElem][innerElem] = distances[innerElem][outerElem] = min;
		}
	    }
	}
    }
}


function keepMerging(clusters) {
    let res = 0;
    for (let c of clusters) {
	if (c.length > 0) {
	    res++;
	}
    }
    if (res > 1) {
	return true;
    } else {
	return false;
    }
}

/**
 * Perform the agglomerative clustering
 * clusters : [][] array of array of elements, initialy one element per cluster
 * distances : [][] array of distance objects (N² could be N*(N-1) if optim needed
 * mergeFunc: function actually performing the merge of clusters, updating distance matrix if needed
 *
 */
function agglomerativeHierarchicalClustering(clusters, distances, mergeFunc) {
    let linkage = [];
    let numElem = clusters.length;
    let numLeft = clusters.length;
    while (keepMerging(clusters)) {
	//while(linkage.length < numElem-1) {
	//console.log(numLeft + " clusters left");
	let merge = findClustersToMerge(distances);
	linkage.push(merge);
	mergeFunc(clusters, distances, merge);
	console.log("Merge " + merge.from + " into " + merge.into + " with dist:" + merge.distance);
    }
    return linkage;
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
    if (typeof linkage === 'undefined') {
	window.linkage = agglomerativeHierarchicalClustering(clusters,
							     flyweightDistances.distances,
							     mergeMinLink);
    } else {
	alert("Clustering deja calculé");
    }
}
