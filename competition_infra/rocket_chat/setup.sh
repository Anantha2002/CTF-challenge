helm repo add stable https://kubernetes-charts.storage.googleapis.com/
helm install --kubeconfig="/home/breadchris/.kube/mcpshsf-cluster-kubeconfig.yaml" --set mongodb.mongodbUsername=rocketchat,mongodb.mongodbPassword=changeme,mongodb.mongodbDatabase=rocketchat,mongodb.mongodbRootPassword=root-changeme --name my-rocketchat stable/rocketchat