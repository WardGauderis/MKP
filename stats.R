library(reshape2)
library(ggplot2)
library(scales)

stats <- read.table("data/local.data", header = T, row.names = 1)
best <- read.table("mkp_instances/best_known_values.txt", header = T, row.names = 1)

value_stats <- stats[, seq(1, 24, by=2)]
time_stats <- stats[, seq(2, 24, by=2)]

relative_percentage_deviation <- function(value) {
  relative <- 100 * (best$best - value) / best$best;
  return (relative);
}

print("AVERAGE PERCENTAGE DEVIATION")
deviations <- relative_percentage_deviation(value_stats);
avg_deviation <- colMeans(deviations[1:30,]);
print(avg_deviation)
avg_deviation <- colMeans(deviations[31:60,]);
print(avg_deviation)
avg_deviation <- colMeans(deviations);
print(avg_deviation)


print("TOTAL COMPUTATION TIME")
total_time <- colSums(time_stats)
print(format( as.POSIXct(Sys.Date())+total_time/1000, "%Mm%OS4s"))

print("TOTAL COMPUTATION TIME 100")
total_time_100 <- colSums(time_stats[1:30,])
print(format( as.POSIXct(Sys.Date())+total_time_100/1000, "%Mm%OS4"))

print("TOTAL COMPUTATION TIME 250")
total_time_250 <- colSums(time_stats[31:60,])
print(format( as.POSIXct(Sys.Date())+total_time_250/1000, "%Mm%OS4"))

print("PERCENTAGE IMPROVED INSTANCES")
better_first <-100 * colMeans(value_stats[, 4:6] > value_stats[, 1:3])
better_best <-100 * colMeans(value_stats[, 7:9] > value_stats[, 1:3])
print(better_first)
print(better_best)

print("PERCENTAGE IMPROVED VND INSTANCES")
better_vnd <-colMeans(value_stats[, 10:12] > value_stats[, 4:6])
print(better_vnd)

print("WILCOX CH")
melted <- melt(deviations[1:30, 1:3])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value)
melted <- melt(deviations[31:60, 1:3])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value)
melted <- melt(deviations[, 1:3])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value)

print("WILCOX II")
melted <- melt(deviations[1:30, 4:9])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value >= 0.05)
melted <- melt(deviations[31:60, 4:9])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value >= 0.05)
melted <- melt(deviations[, 4:9])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value)

print("WILCOX VND")
melted <- melt(deviations[1:30, 10:12])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value)
melted <- melt(deviations[31:60, 10:12])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value >= 0.05)
melted <- melt(deviations[, 10:12])
w <- pairwise.wilcox.test(melted$value, melted$variable, paired = T)
print(w$p.value)

boxplot(deviations, xlab="Algorithm", ylab="Relative percentage deviation",
        names = c("R", "G", "T", "FI-R", "FI-G", "FI-T", "BI-R", "BI-G", "BI-T", "VND-R", "VND-G", "VND-T"), las=2)
boxplot(time_stats, xlab="Algorithm", ylab="Total computation time",
        log = "y",names = c("R", "G", "T", "FI-R", "FI-G", "FI-T", "BI-R", "BI-G", "BI-T", "VND-R", "VND-G", "VND-T"), las=2)

m <- melt(deviations)
ggplot(m, aes(variable, value)) + geom_boxplot() + theme_linedraw() +
  scale_x_discrete(labels=c("Random", "Greedy", "Toyoda", "FI Random", "FI Greedy", "FI Toyoda", "BI Random", "BI Greedy", "BI Toyoda", "VND Random", "VND Greedy", "VND Toyoda")) +
  theme(axis.text.x = element_text(angle = 45, hjust=1)) +
  labs(x="Algorithm", y="Relative percentage deviation")

m <- melt(time_stats)
ggplot(m, aes(variable, value)) + geom_boxplot() + theme_linedraw() +
  scale_x_discrete(labels=c("Random", "Greedy", "Toyoda", "FI Random", "FI Greedy", "FI Toyoda", "BI Random", "BI Greedy", "BI Toyoda", "VND Random", "VND Greedy", "VND Toyoda")) +
  theme(axis.text.x = element_text(angle = 45, hjust=1)) +
  labs(x="Algorithm", y="Computation time (ms)") + 
  scale_y_log10()