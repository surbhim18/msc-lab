#uses data in dataset.txt
dataset <- read.table("dataset.txt",header=TRUE,row.names=1)

#find min y from all plots, max y from all plots
xmin <- min(dataset[,1])
xmax <- max(dataset[,1])
ymin <- min(dataset[,2])
ymax <- max(dataset[,2])

xlim = c(xmin,xmax)
ylim = c(ymin,ymax)

#plotting all points in one graph with different colors
color <- c("blue","red","green","yellow","black")

plot(dataset[,1], dataset[,2], xlab="X", ylab="Y", col = ifelse(dataset[,3] == 1, color[1], ifelse(dataset[,3] == 2, color[2], ifelse(dataset[,3] == 3, color[3],ifelse(dataset[,3] == 4, color[4],color[5])))))

legend(xlim[2]-6, ylim[2]-10, legend=c("G1","G2","G3","Exp","Uni") , cex=0.8, pch = 1, col=color, title="Data points")

#CLUSTERING
require(graphics)

#dataset without class labels
cdataset = dataset[,-3]

#FUNCTIONS
#Square
square <- function(x)
{
  squared <- (x)*(x)
  return(squared)
}


#Purity
cal_purity <- function(new_labs, old_labs) 
{
  purity <- sum(apply(table(old_labs, new_labs), 2, max)) / length(new_labs)
  return (purity)
}

#Purity without noise points
cal_purity_noise <- function(new_labs, old_labs) 
{
  remove <- c(0)
  indices <- new_labs %in% remove
  new_labs <- new_labs[!indices]
  old_labs <- old_labs[!indices]
  
  purity <- sum(apply(table(old_labs, new_labs), 2, max)) / length(new_labs)
  return (purity)
}

#SSE
cal_SSE <- function(dataset, new_labs, k)
{
  sum_x <- vector(mode = "numeric", length = k)
  sum_y <- vector(mode = "numeric", length = k)
  nc <- vector(mode = "numeric", length = k)
  n = nrow(dataset)
  
  for (i in 1:n)
  {
    if(new_labs[i] == 0) { next }
    
    sum_x[new_labs[i]] <- sum_x[new_labs[i]] + dataset[i,1]
    sum_y[new_labs[i]] <- sum_y[new_labs[i]] + dataset[i,2]
    nc[new_labs[i]] <- nc[new_labs[i]] + 1
  }
  mean_x <- sum_x/nc
  mean_y <- sum_y/nc
  
  err_x <- 0
  err_y <- 0
  for (i in 1:n)
  {
    if(new_labs[i] == 0) { next }
    
    err_x <- err_x + square(mean_x[new_labs[i]] - dataset[i,1])
    err_y <- err_y + square(mean_y[new_labs[i]] - dataset[i,2])
  }
  
  sse <- err_x + err_y
  return (sse)
}

#K-MEANS
#kmeans(x, centers, iter.max = 10)
cluster_size <- 5
clk <- kmeans(cdataset, cluster_size, 10)
plot(cdataset, col = clk$cluster, pch = clk$cluster, xlab="X", ylab="Y")
points(clk$centers, col = "black" , pch = 8, cex = 1)
title("K-means")
legend(xlim[2]-6, ylim[2]-10, legend = sort(unique(clk$cluster)) , cex=0.8, col = sort(unique(clk$cluster)), pch = sort(unique(clk$cluster)), title="Clusters")
purity <- round(cal_purity(clk$cluster,dataset[,3]),3)
sse <- round(cal_SSE(cdataset,clk$cluster, cluster_size),3)
val <- paste(paste("SSE = ",sse),paste("\nPurity = ",purity))
text(xlim[2]-12, ylim[1]+3, val , cex=0.8)


#HIERARCHICHAL CLUSTERING
cluster_size <- 5
hc <- hclust(dist(cdataset), "ward.D")
clh <- cutree(as.hclust(hc), k = cluster_size)
plot(cdataset, col = clh, pch = clh, xlab="X", ylab="Y")
title("Heirarchial Clustering")
legend(xlim[2]-6, ylim[2]-10, legend = sort(unique(clh)) , cex=0.8, col = sort(unique(clh)), pch = sort(unique(clh)), title="Clusters")
purity <-round(cal_purity(clh,dataset[,3]),3)
sse <- round(cal_SSE(cdataset,clh, cluster_size),3)
val <- paste(paste("SSE = ",sse),paste("\nPurity = ",purity))
text(xlim[2]-12, ylim[1]+3, val , cex=0.8)

#DBSCAN
#downloaded using install.packages("dbscan")
library(dbscan)
kNNdistplot(cdataset, k = 5)
db <- dbscan(cdataset, eps = 1.5, minPts = 5)
cld <- db$cluster
hullplot(cdataset, db)
plot(cdataset, col = cld+1, pch = cld+1, xlab="X", ylab="Y")
title("DBScan")
cluster_size <- length(unique(cld)) - 1
legend(xlim[2]-6, ylim[2]-4, legend = sort(unique(cld)) , cex=0.8, col = sort(unique(cld + 1)), pch = sort(unique(cld + 1)), title="Clusters")
purity <- round(cal_purity(cld,dataset[,3]),3)
purity_noise <- round(cal_purity_noise(cld,dataset[,3]),3)
sse <- round(cal_SSE(cdataset,cld, cluster_size),3)
val <- paste(paste("SSE = ",sse),paste("\nPurity = ",purity),paste("\nPurity (w/o noise) = ",purity_noise))
text(xlim[2]-15, ylim[1]+3, val , cex=0.8)

